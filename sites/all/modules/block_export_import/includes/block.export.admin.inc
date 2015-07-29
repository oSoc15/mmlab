<?php

/**
 * @file
 * Export all system specific blocks.
 */

/**
 * Form constructor for export all system specific blocks array structure.
 *
 * @see block_export_import_blocks_export_form_submit()
 * @see _get_importable_custom_block_object()
 * @see _export_custom_blocks_form_validate()
 */
function block_export_import_blocks_export_form($form, &$form_state) {
  $block_export_import_path = drupal_get_path('module', 'block_export_import');
  $form['#attached']['css'] = array(
    $block_export_import_path . '/css/block_export_import.css',
  );

  // A condition to verify form element export_code is empty.
  if (empty($form_state['storage']['values'])) {
    // This query is used to fetch all system specific block.
    $query = db_select('block_custom', 'block_custom')->orderBy('block_custom.info', 'ASC');
    $query->fields('block_custom', array('bid', 'info'));
    $result = $query->execute();
    // Get the count value form result set.
    $row_count = $result->rowCount();
    if ($row_count) {
      $custom_block_prefix = '';
      $custom_block_prefix .= '<div class="block_name">Block Name</div>';
      $custom_block_prefix .= '<div class="export_type">Export Type</div>';
      $custom_block_prefix .= '<div class="block_action">Action</div>';
      $form['custom_block'] = array(
        '#tree' => TRUE,
        '#prefix' => '<div class="header">' . $custom_block_prefix,
        '#suffix' => '</div><div style="clear:both;"></div>',
      );

      // $counter is used to set the alternate color on extracted rows.
      $counter = 0;

      // Iterate over each element in our $form['custom_block'] array.
      foreach ($result as $data) {
        $class = 'odd';
        if ($counter++ % 2 != 0) {
          $class = 'even';
        }

        // Block Description.
        $block_title = $data->info;
        // The block’s block.bid.
        $block_id = $data->bid;

        $form['custom_block'][$block_id]['export'] = array(
          '#type' => 'checkbox',
          '#title' => $block_title,
          '#prefix' => '<div class="' . $class . ' ">',
        );

        $form['custom_block'][$block_id]['export_type'] = array(
          '#type' => 'radios',
          '#title' => $block_title,
          '#options' => array('basic' => t('Basic Information'), 'full' => 'Full Information'),
          '#title_display' => 'invisible',
          '#default_value' => 'full',
        );

        $op_link = 'admin/structure/block/manage/block/' . $block_id;
        $op_link_attr = array('attributes' => array('target' => '_blank'));
        $form['custom_block'][$block_id]['operation'] = array(
          '#type' => 'item',
          '#markup' => l(t('Edit block'), $op_link, $op_link_attr),
          '#suffix' => '<div style="clear:left;"></div></div>',
        );
      }

      $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Export Blocks'),
        '#validate' => array('_block_export_import_blocks_export_form_validate'),
        '#prefix' => '<div class="export-block-submit">',
        '#suffix' => '</div>',
      );
    }
    else {
      $form['custom_block']['no_block'] = array(
        '#type' => 'item',
        '#markup' => t('No system specific block exists.'),
      );
    }
  }
  else {
    // Get Importable array structure as string.
    $export_data = _block_export_import_get_importable_object($form_state);

    $export_desc = "For importing copy the content of the text area and paste";
    $export_desc .= " it into the import page.";
    $form['export_code'] = array(
      '#type' => 'textarea',
      '#title' => t('Export'),
      '#default_value' => $export_data,
      '#rows' => 15,
      '#description' => $export_desc,
    );

    $link = l(t('Export Blocks'), 'admin/structure/export-import-block/export');
    $form['back'] = array(
      '#type' => 'item',
      '#markup' => $link,
    );
  }
  return $form;
}

/**
 * Use to validate user must be selected at least one block to export.
 */
function _block_export_import_blocks_export_form_validate($form, $form_state) {
  $custom_block = $form_state['values']['custom_block'];
  $flag = FALSE;
  foreach ($custom_block as $data) {
    if ($data['export']) {
      $flag = TRUE;
      break;
    }
  }

  if ($flag == FALSE) {
    form_set_error('custom_block', t('Please select at least one block.'));
  }
}

/**
 * Export an array object of all selected system specific blocks.
 *
 * @return array
 *   Formatted structured array for import.
 */
function _block_export_import_get_importable_object($form_state) {
  // Verify that input text area have some code.
  if (!empty($form_state['storage']['values'])) {
    // Use to fetch all fields ffrom block table.
    $block_schema_fields = _block_export_import_get_block_schema_fields();
    // Use to store an structured array as an string.
    $export_data = '';
    // Append the blocks array.
    $export_data = "\$block = array();\n";

    // Iterate for each selected block.
    foreach ($form_state['storage']['values'] as $block_id => $storage_data) {
      if ($storage_data['export']) {
        // This query is used to fetch basic or full information of
        // seleted blocks.
        $query = db_select('block_custom', 'block_custom');
        $query->fields('block_custom', array('info', 'body', 'format'));
        if ($storage_data['export_type'] == 'full') {
          $query->innerJoin('block', 'block',
            'block.delta = block_custom.bid');
          $query->leftJoin('block_node_type', 'block_node_type',
            'block_custom.bid = block_node_type.delta');
          $query->fields('block_node_type', array('type'));
          $query->leftJoin('block_role', 'block_role',
            'block_custom.bid = block_role.delta');
          $query->fields('block_role', array('rid'));
          $query->fields('block', $block_schema_fields);
        }

        $query->condition('block_custom.bid', $block_id);
        $result = $query->execute();
        // Get the count value form result set.
        $row_count = $result->rowCount();
        if ($row_count) {
          $block_obj = array();
          // Iterate through each database result.
          foreach ($result as $key => $data) {
            // It may be basic or full only.
            $block_obj['data']['export_type'] = $storage_data['export_type'];
            // The Block description.
            $block_obj['data']['info'] = $data->info;
            // The Block body contents.
            $block_obj['data']['body'] = addslashes($data->body);
            // The filter_format.format of the block body.
            $block_obj['data']['format'] = $data->format;
            if ($storage_data['export_type'] == 'full') {
              // The Flag to indicate how users may control visibility of
              // the block.
              $block_obj['data']['vsettings']['custom'] = $data->custom;

              // Flag to indicate how to show blocks on pages.
              $block_obj['data']['vsettings']['visibility'] = $data->visibility;
              // The Contents of the "Pages" block; contains either a list of
              // paths on which to include/exclude the block or PHP code,
              // depending on "visibility" setting.
              $block_obj['data']['vsettings']['pages'] = $data->pages;
              if ((int) $data->rid) {
                $rid = $data->rid;
                // The result may have some duplicate records but here we are
                // overriding it by same key of block_role array.
                // The user’s role ID.
                $block_obj['data']['vsettings']['role'][$rid] = $rid;
              }
              if ($data->type) {
                $type = $data->type;
                // The result may have some duplicate records but here we are
                // overriding it by same key of block_node_type array.
                // The machine-readable name of  conten type.
                $block_obj['data']['vsettings']['node_type'][$type] = $type;
              }
              // Custom title for the block.
              $block_obj['data']['osettings']['title'] = $data->title;
              if (array_key_exists('css_class', $data)) {
                $block_obj['data']['osettings']['css_class'] = $data->css_class;
              }
            }
          }

          // Iterate through each block structured array.
          foreach ($block_obj as $data) {
            $export_data .= "\$block[] = array(\n";
            // Append Basic Information.
            $export_data .= " 'block_custom' => array(\n";
            // Use for determine type of import.
            $export_data .= " 'export_type' => '" . $data['export_type'] . "', \n";
            $export_data .= " 'info' => '" . $data['info'] . "', \n";
            $export_data .= " 'format' => '" . $data['format'] . "', \n";
            $export_data .= " 'body' => '" . $data['body'] . "', \n";
            $export_data .= " ),\n";

            if ($data['export_type'] == 'full') {
              if (array_key_exists('vsettings', $data)) {
                $visibility_data = $data['vsettings'];
                $export_data .= '/* Visibility settings */' . "\n";

                // Append Visibility settings.
                $export_data .= " 'visibility_settings' =>  array(";
                foreach ($visibility_data as $key => $vdata) {
                  switch ($key) {
                    case 'custom':
                      $export_data .= " 'custom' => " . (int) $vdata . ", \n";
                      break;

                    case 'visibility':
                      $export_data .= " 'visibility' => " . (int) $vdata . ", \n";
                      break;

                    case 'pages':
                      $export_data .= " 'pages' => '" . $vdata . "', \n";
                      break;

                    case 'role':
                      $export_data .= " 'block_role' => array(\n";
                      foreach ($vdata as $role_id) {
                        $export_data .= "  " . (int) $role_id . ", \n";
                      }
                      $export_data .= " ),\n";
                      break;

                    case 'node_type':
                      $export_data .= " 'block_node_type' => array(\n";
                      foreach ($vdata as $node_type) {
                        $export_data .= "  '" . $node_type . "', \n";
                      }
                      $export_data .= " ),\n";
                      break;
                  }
                }
                $export_data .= "), \n";
              }

              if (array_key_exists('osettings', $data)) {
                $other_data = $data['osettings'];
                $export_data .= '/* Other Settings */' . "\n";

                // Append other block settings.
                $export_data .= " 'other_settings' => array(\n";
                foreach ($other_data as $key => $odata) {
                  switch ($key) {
                    case 'title':
                      $export_data .= " 'title' => '" . $odata . "', \n";
                      break;

                    case 'css_class':
                      $export_data .= " 'css_class' => '" . $odata . "', \n";
                      break;
                  }
                }
                $export_data .= " ), \n";
              }
            }
            $export_data .= ");\n\n";
          }
        }
      }
    }
    return $export_data;
  }
}

/**
 * Submit handler for blockexport_export_blocks_form_submit() to save a blocks.
 */
function block_export_import_blocks_export_form_submit($form, &$form_state) {
  if (empty($form_state['storage']['values'])) {
    // If there is no previous values redraw for next step.
    $form_state['storage']['values'] = $form_state['values']['custom_block'];
    $form_state['rebuild'] = TRUE;
  }
  else {
    // Form is on the next step, process the data here.
  }
}
