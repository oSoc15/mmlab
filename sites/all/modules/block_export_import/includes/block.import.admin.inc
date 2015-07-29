<?php

/**
 * @file
 * Import all system specific blocks.
 */

/**
 * Form constructor for import system specific blocks.
 *
 * @see block_export_import_blocks_import_form_submit()
 * @see import_block_form_submit()
 */
function block_export_import_blocks_import_form($form, &$form_state) {
  $block_export_import_path = drupal_get_path('module', 'block_export_import');
  $form['#attached']['css'] = array(
    $block_export_import_path . '/css/block_export_import.css',
  );

  if (empty($form_state['storage']['values'])) {
    $form['import_code'] = array(
      '#type' => 'textarea',
      '#title' => t('Import'),
      '#required' => TRUE,
      '#rows' => 15,
      '#description' => t('Paste an exported blocks array structure here.'),
    );

    $override_desc = "If selected 'Yes', any existing System Block with";
    $override_desc .= " the same identifier will be replaced by the import.";
    $form['override_existing'] = array(
      '#type' => 'radios',
      '#title' => t('Override'),
      '#options' => array(1 => t('Yes'), -1 => t('No')),
      '#required' => TRUE,
      '#description' => $override_desc,
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Import Blocks'),
    '#prefix' => '<div class="import-block-submit">',
    '#suffix' => '</div>',
  );

  return $form;
}

/**
 * Submit handler for import_custom_blocks_form() to import/save a block.
 *
 * @see block_export_import_block_import_process()
 * @see _block_export_import_validate_import_data()
 */
function block_export_import_blocks_import_form_submit($form, &$form_state) {
  if (empty($form_state['storage']['values'])) {
    $override_existing  = $form_state['values']['override_existing'];
    $block = array();
    $string = $form_state['values']['import_code'];
    @eval($string);

    if (is_array($block)) {
      $php_filter_module_status = module_exists('php');
      foreach ($block as $block_data) {
        // Validate array structure.
        $is_comptable = _block_export_import_validate_import_data($block_data);
        if ($is_comptable) {
          if (empty($block_data['block_custom']['info'])) {
            $error = t('Please ensure that each block description is unique.');
            form_set_error('info', $error);
          }
          else {
            block_export_import_block_import_process($block_data, $override_existing, $php_filter_module_status);
          }
        }
      }
      cache_clear_all();
      $form_state['rebuild'] = TRUE;
    }
  }
  else {
    // Do nothing.
  }
  drupal_goto('admin/structure/export-import-block/export');
}

/**
 * The Process steps to create a system specific block.
 *
 * @see _block_export_import_get_block_fields()
 * @see _block_export_import_import_visibility_settings()
 */
function block_export_import_block_import_process($block_data, $override_existing, $php_filter_module_status) {
  // It is use to verify related module are enabled or not if not enabled then
  // it throw an error,for a particular block.
  $bypass_flag = TRUE;

  // The variable to display all types of messages which are produce in
  // the blocks creation process.
  $message = "";

  // An array of block basic information.
  $block_custom = $block_data['block_custom'];
  // Variable to verify type of import is basic or full.
  $export_type = $block_custom['export_type'];
  // Block description.
  $block_desc = $block_custom['info'];
  // The filter_format format of the block body.
  $format = $block_custom['format'];
  // The Block contents.
  $body = stripslashes($block_custom['body']);

  // Query to verify that this block is exists or not.
  $query = "SELECT bid AS block_id FROM {block_custom} WHERE info = :info";
  $block_id = db_query($query, array(':info' => $block_desc))->fetchField();
  if ($block_id > 0) {
    // Override the exisitng block.
    if ($override_existing == 1) {
      if ($export_type == 'full') {
        if ($block_data['visibility_settings']['visibility'] == 2) {
          if (!$php_filter_module_status) {
            $block_desc = $block_data['block_custom']['info'];
            $mes = t("The PHP filter module should be enabled to&nbsp;");
            $mes .= t("override the block &nbsp;");
            $mes .= t("'!block_info'", array('!block_info' => $block_desc));
            drupal_set_message($mes, 'error');
            $bypass_flag = FALSE;
          }
        }
      }

      if ($bypass_flag) {
        // Update custom block's basic information.
        db_update('block_custom')->fields(
          array(
            'body' => $body,
            'format' => $format,
          )
        )->condition('info', $block_desc)->execute();

        if ($export_type == 'full') {
          $block_fields = array();
          $fields = _block_export_import_get_block_fields($block_data, $block_fields);

          // Update block's Visibility settings, title etc.
          db_update('block')->fields($fields)->condition('delta', $block_id)
            ->execute();

          // Detete the existing content type form same block to this block to
          // override by new settings.
          db_delete('block_node_type')->condition('module', 'block')
            ->condition('delta', $block_id)->execute();

          // Detete the existing roles form same block to this block to
          // override by new settings.
          db_delete('block_role')->condition('module', 'block')
            ->condition('delta', $block_id)->execute();

          // Use to override the block content type and roles settings.
          _block_export_import_import_visibility_settings($block_data, $block_id);
        }

        $message = t("Block '!block_info' has been override successfully.",
                      array('!block_info' => $block_desc)
                    );
        drupal_set_message($message);
      }
    }
  }
  else {
    if ($export_type == 'full') {
      if ($block_data['visibility_settings']['visibility'] == 2) {
        if (!$php_filter_module_status) {
          $block_desc = $block_data['block_custom']['info'];
          $mes = t("The PHP filter module should be enabled to&nbsp;");
          $mes .= t("import the block &nbsp;");
          $mes .= t("'!block_info'", array('!block_info' => $block_desc));
          drupal_set_message($mes, 'error');
          $bypass_flag = FALSE;
        }
      }
    }

    if ($bypass_flag) {
      // Create a new system specific blocks and return the current block id.
      $block_id = db_insert('block_custom')
        ->fields(array(
          'info' => $block_desc,
          'body' => $body,
          'format' => $format,
          )
        )->execute();

      // Returns a list of all currently available themes.
      $available_themes = list_themes();
      foreach ($available_themes as $theme) {
        if ($theme->status) {
          // Initialize a blank array to set the default value of block fields.
          $block_fields = array();
          $block_fields['module'] = 'block';
          $block_fields['delta'] = $block_id;
          $block_fields['theme'] = $theme->name;
          $block_fields['status'] = 0;
          $block_fields['weight'] = 0;
          $block_fields['custom'] = 0;
          $block_fields['visibility'] = 0;
          $block_fields['pages'] = '';
          $block_fields['title'] = '';
          $block_fields['cache'] = -1;
          if ($export_type == 'full') {
            // This is use to override the default value of block fields
            // by imported code.
            $block_fields = _block_export_import_get_block_fields($block_data, $block_fields);
          }
          // Insert the specific block entry into this table.
          db_insert('block')->fields($block_fields)->execute();
        }
      }

      if ($export_type == 'full') {
        // Set the block visibility settings.
        _block_export_import_import_visibility_settings($block_data, $block_id);
      }

      $message = t("Block '!block_info' has been created successfully.",
                      array('!block_info' => $block_desc)
                    );
      drupal_set_message($message);
      return;
    }
  }
}

/**
 * This is use to return an array to set an entry into block table.
 *
 * @return array
 *   An block table key => value array.
 */
function _block_export_import_get_block_fields($block_data, $block_fields) {
  $visibility_data = $block_data['visibility_settings'];
  if (count($visibility_data)) {
    foreach ($visibility_data as $key => $data) {
      switch ($key) {
        case 'custom':
          $block_fields['custom'] = $data;
          break;

        case 'visibility':
          $block_fields['visibility'] = $data;
          break;

        case 'pages':
          $block_fields['pages'] = $data;
          break;
      }
    }
  }

  $other_settings = $block_data['other_settings'];
  if (count($other_settings)) {
    foreach ($other_settings as $key => $data) {
      switch ($key) {
        case 'title':
          $block_fields['title'] = $data;
          break;

        case 'css_class':
          if (module_exists('block_class')) {
            $block_fields['css_class'] = $data; 
          }
          break;
      }
    }
  }
  return $block_fields;
}

/**
 * Save handler for to save Visibility settings content types.
 */
function _block_export_import_import_node_type($node_types, $block_id) {
  $query = db_select('node_type', 'node_type');
  $query->fields('node_type', array('type'));
  $query->condition('node_type.type', $node_types, 'IN');
  $result = $query->execute();
  // Get the count value form the result set.
  $row_count = $result->rowCount();
  if ($row_count) {
    // Iterate through each database result set.
    foreach ($result as $data) {
      db_merge('block_node_type')
        ->key(array('delta' => $block_id, 'type' => $data->type))
        ->fields(array(
            'module' => 'block',
            'delta' => $block_id,
            'type' => $data->type,
          )
        )->execute();
    }
  }
}

/**
 * Save handler for to save Visibility settings roles.
 */
function _block_export_import_import_roles($roles, $block_id) {
  $query = db_select('role', 'role');
  $query->fields('role', array('rid'));
  $query->condition('role.rid', $roles, 'IN');
  $result = $query->execute();
  // Get the count value form the result set.
  $row_count = $result->rowCount();
  if ($row_count) {
    // Iterate through each database result set.
    foreach ($result as $data) {
      db_merge('block_role')
        ->key(array('delta' => $block_id, 'rid' => $data->rid))
        ->fields(array(
          'module' => 'block',
          'delta' => $block_id,
          'rid' => $data->rid,
          )
        )->execute();
    }
  }
}

/**
 * Save handler for to save Visibility settings content types and roles.
 *
 * @see _block_export_import_import_roles()
 * @see _block_export_import_import_node_type()
 */
function _block_export_import_import_visibility_settings($block_data, $block_id) {
  $visibility_settings = $block_data['visibility_settings'];
  if (count($visibility_settings)) {
    foreach ($visibility_settings as $key => $data) {
      switch ($key) {
        case 'block_role':
          $roles = $visibility_settings['block_role'];
          _block_export_import_import_roles($roles, $block_id);
          break;

        case 'block_node_type':
          $node_types = $visibility_settings['block_node_type'];
          _block_export_import_import_node_type($node_types, $block_id);
          break;
      }
    }
  }
}

/**
 * Import array structure validate handlor.
 */
function _block_export_import_validate_import_data($block_data) {
  $flag = FALSE;
  $array_types[] = array(
    'block_custom' => array(
      'export_type' => '',
      'info' => '',
      'format' => '',
      'body' => '',
    ),
  );

  $array_types[] = array(
    'block_custom' => array(
      'export_type' => '',
      'info' => '',
      'format' => '',
      'body' => '',
    ),
    'visibility_settings' => array(
      'custom' => 1,
      'visibility' => 1,
      'pages' => 'node/1',
      'block_role' => array(1,
      ),
      'block_node_type' => array('',
      ),
    ),
    'other_settings' => array('title' => '',
    ),
  );

  $array_types[] = array(
    'block_custom' => array(
      'export_type' => '',
      'info' => '',
      'format' => '',
      'body' => '',
    ),
    'visibility_settings' => array(
      'custom' => 1,
      'visibility' => 1,
      'pages' => 'node/1',
      'block_role' => array(1,
      ),
      'block_node_type' => array('',
      ),
    ),
    'other_settings' => array(
      'title' => '',
      'css_class' => '',
    ),
  );

  $array_types[] = array(
    'block_custom' => array(
      'export_type' => '',
      'info' => '',
      'format' => '',
      'body' => '',
    ),
    'visibility_settings' => array(
      'custom' => 1,
      'visibility' => 1,
      'pages' => 'node/1',
    ),
    'other_settings' => array('title' => '',
    ),
  );

  $array_types[] = array(
    'block_custom' => array(
      'export_type' => '',
      'info' => '',
      'format' => '',
      'body' => '',
    ),
    'visibility_settings' => array(
      'custom' => 1,
      'visibility' => 1,
      'pages' => 'node/1',
    ),
    'other_settings' => array(
      'title' => '',
      'css_class' => '',
    ),
  );

  $array_types[] = array(
    'block_custom' => array(
      'export_type' => '',
      'info' => '',
      'format' => '',
      'body' => '',
    ),
    'visibility_settings' => array(
      'custom' => 1,
      'visibility' => 1,
      'pages' => 'node/1',
      'block_role' => array(1,
      ),
    ),
    'other_settings' => array('title' => '',
    ),
  );

  $array_types[] = array(
    'block_custom' => array(
      'export_type' => '',
      'info' => '',
      'format' => '',
      'body' => '',
    ),
    'visibility_settings' => array(
      'custom' => 1,
      'visibility' => 1,
      'pages' => 'node/1',
      'block_role' => array(1,
      ),
    ),
    'other_settings' => array(
      'title' => '',
      'css_class' => '',
    ),
  );

  $array_types[] = array(
    'block_custom' => array(
      'export_type' => '',
      'info' => '',
      'format' => '',
      'body' => '',
    ),
    'visibility_settings' => array(
      'custom' => 1,
      'visibility' => 1,
      'pages' => '',
      'block_node_type' => array('',
      ),
    ),
    'other_settings' => array('title' => '',
    ),
  );

  $array_types[] = array(
    'block_custom' => array(
      'export_type' => '',
      'info' => '',
      'format' => '',
      'body' => '',
    ),
    'visibility_settings' => array(
      'custom' => 1,
      'visibility' => 1,
      'pages' => '',
      'block_node_type' => array('',
      ),
    ),
    'other_settings' => array(
      'title' => '',
      'css_class' => '',
    ),
  );

  foreach ($array_types as $item) {
    $arr = array_diff_key($block_data, $item);
    if (count($arr) == 0) {
      $flag = TRUE;
    }
  }
  return $flag;
}
