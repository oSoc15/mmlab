<?php

/**
 * @file
 * Template overrides as well as (pre-)process and alter hooks for the
 * {{ THEME NAME }} theme.
 */
function newmedialab_superfish_build($variables) {
    $output = array('content' => '');
    $id = $variables['id'];
    $menu = $variables['menu'];
    $depth = $variables['depth'];
    $trail = $variables['trail'];
    // Keep $sfsettings untouched as we need to pass it to the child menus.
    $settings = $sfsettings = $variables['sfsettings'];
    $megamenu = $megamenu_below = $settings['megamenu'];
    $total_children = $parent_children = $single_children = 0;
    $i = 1;

    // Reckon the total number of available menu items.
    foreach ($menu as $menu_item) {
        if (!isset($menu_item['link']['hidden']) || $menu_item['link']['hidden'] == 0) {
            $total_children++;
        }
    }

    foreach ($menu as $menu_item) {

        $show_children = $megamenu_wrapper = $megamenu_column = $megamenu_content = FALSE;
        $item_class = $link_options = $link_class = array();
        $mlid = $menu_item['link']['mlid'];

        if (!isset($menu_item['link']['hidden']) || $menu_item['link']['hidden'] == 0) {
            $item_class[] = ($trail && in_array($mlid, $trail)) ? 'active-trail' : '';

            // Add helper classes to the menu items and hyperlinks.
            $settings['firstlast'] = ($settings['dfirstlast'] == 1 && $total_children == 1) ? 0 : $settings['firstlast'];
            $item_class[] = ($settings['firstlast'] == 1) ? (($i == 1 && $i == $total_children) ? 'firstandlast' : (($i == 1) ? 'first' : (($i == $total_children) ? 'last' : 'middle'))) : '';
            $settings['zebra'] = ($settings['dzebra'] == 1 && $total_children == 1) ? 0 : $settings['zebra'];
            $item_class[] = ($settings['zebra'] == 1) ? (($i % 2) ? 'odd' : 'even') : '';
            $item_class[] = ($settings['itemcount'] == 1) ? 'sf-item-' . $i : '';
            $item_class[] = ($settings['itemdepth'] == 1) ? 'sf-depth-' . $menu_item['link']['depth'] : '';
            $link_class[] = ($settings['itemdepth'] == 1) ? 'sf-depth-' . $menu_item['link']['depth'] : '';
            $item_class[] = ($settings['liclass']) ? $settings['liclass'] : '';
            if (strpos($settings['hlclass'], ' ')) {
                $l = explode(' ', $settings['hlclass']);
                foreach ($l as $c) {
                    $link_class[] = $c;
                }
            }
            else {
                $link_class[] = $settings['hlclass'];
            }
            $i++;

            // Hide hyperlink descriptions ("title" attribute).
            if ($settings['hidelinkdescription'] == 1) {
                unset($menu_item['link']['localized_options']['attributes']['title']);
            }

            // Insert hyperlink description ("title" attribute) into the text.
            $show_linkdescription = ($settings['linkdescription'] == 1 && !empty($menu_item['link']['localized_options']['attributes']['title'])) ? TRUE : FALSE;
            if ($show_linkdescription) {
                if (!empty($settings['hldmenus'])) {
                    $show_linkdescription = (is_array($settings['hldmenus'])) ? ((in_array($mlid, $settings['hldmenus'])) ? TRUE : FALSE) : (($mlid == $settings['hldmenus']) ? TRUE : FALSE);
                }
                if (!empty($settings['hldexclude'])) {
                    $show_linkdescription = (is_array($settings['hldexclude'])) ? ((in_array($mlid, $settings['hldexclude'])) ? FALSE : $show_linkdescription) : (($settings['hldexclude'] == $mlid) ? FALSE : $show_linkdescription);
                }
                if ($show_linkdescription) {
                    $menu_link_title = $menu_item['link']['title'];
                    $menu_item['link']['title'] = '<span class="sf-title">' . $menu_link_title . '</span>';
                    $menu_item['link']['title'] .= ' <span class="sf-description">';
                    $menu_item['link']['title'] .= (!empty($menu_item['link']['localized_options']['attributes']['title'])) ? $menu_item['link']['localized_options']['attributes']['title'] : array();
                    $menu_item['link']['title'] .= '</span>';
                    $link_options['html'] = TRUE;
                }
            }

            // Add custom HTML codes around the menu items.
            if ($sfsettings['wrapul'] && strpos($sfsettings['wrapul'], ',') !== FALSE) {
                $wul = explode(',', $sfsettings['wrapul']);
                // In case you just wanted to add something after the element.
                if (drupal_substr($sfsettings['wrapul'], 0) == ',') {
                    array_unshift($wul, '');
                }
            }
            else {
                $wul = array();
            }

            // Add custom HTML codes around the hyperlinks.
            if ($settings['wraphl'] && strpos($settings['wraphl'], ',') !== FALSE) {
                $whl = explode(',', $settings['wraphl']);
                // The same as above
                if (drupal_substr($settings['wraphl'], 0) == ',') {
                    array_unshift($whl, '');
                }
            }
            else {
                $whl = array();
            }

            // Add custom HTML codes around the hyperlinks text.
            if ($settings['wraphlt'] && strpos($settings['wraphlt'], ',') !== FALSE) {
                $whlt = explode(',', $settings['wraphlt']);
                // The same as above
                if (drupal_substr($settings['wraphlt'], 0) == ',') {
                    array_unshift($whlt, '');
                }
                $menu_item['link']['title'] = $whlt[0] . check_plain($menu_item['link']['title']) . $whlt[1];
                $link_options['html'] = TRUE;
            }

            $expanded = ($sfsettings['expanded'] == 1) ? (($menu_item['link']['expanded'] == 1) ? TRUE : FALSE) : TRUE;

            if (!empty($menu_item['link']['has_children']) && !empty($menu_item['below']) && $depth != 0 && $expanded === TRUE) {

                // Megamenu is still beta, there is a good chance much of this will be changed.
                if (!empty($settings['megamenu_exclude'])) {
                    if (is_array($settings['megamenu_exclude'])) {
                        $megamenu_below = (in_array($mlid, $settings['megamenu_exclude'])) ? 0 : $megamenu;
                    }
                    else {
                        $megamenu_below = ($settings['megamenu_exclude'] == $mlid) ? 0 : $megamenu;
                    }
                    // Send the result to the sub-menu.
                    $sfsettings['megamenu'] = $megamenu_below;
                }
                if ($megamenu_below == 1) {
                    $megamenu_wrapper = ($menu_item['link']['depth'] == $settings['megamenu_depth']) ? TRUE : FALSE;
                    $megamenu_column = ($menu_item['link']['depth'] == $settings['megamenu_depth'] + 1) ? TRUE : FALSE;
                    $megamenu_content = ($menu_item['link']['depth'] >= $settings['megamenu_depth'] && $menu_item['link']['depth'] <= $settings['megamenu_levels']) ? TRUE : FALSE;
                }
                // Render the sub-menu.
                $var = array(
                    'id' => $id,
                    'menu' => $menu_item['below'],
                    'depth' => $depth,
                    'trail' => $trail,
                    'sfsettings' => $sfsettings
                );
                $children = theme('superfish_build', $var);
                // Check to see whether it should be displayed.
                $show_children = (($menu_item['link']['depth'] <= $depth || $depth == -1) && $children['content']) ? TRUE : FALSE;
                if ($show_children) {
                    // Add item counter classes.
                    if ($settings['itemcounter']) {
                        $item_class[] = 'sf-total-children-' . $children['total_children'];
                        $item_class[] = 'sf-parent-children-' . $children['parent_children'];
                        $item_class[] = 'sf-single-children-' . $children['single_children'];
                    }
                    // More helper classes.
                    $item_class[] = ($megamenu_column) ? 'sf-megamenu-column' : '';
                    $item_class[] = $link_class[] = 'menuparent';
                }
                $parent_children++;
            }
            else {
                $item_class[] = 'sf-no-children';
                $single_children++;
            }

            $item_class = implode(' ', array_remove_empty($item_class));

            if (isset($menu_item['link']['localized_options']['attributes']['class'])) {
                $link_class_current = $menu_item['link']['localized_options']['attributes']['class'];
                $link_class = array_merge($link_class_current, array_remove_empty($link_class));
            }
            $menu_item['link']['localized_options']['attributes']['class'] = array_remove_empty($link_class);

            // The Context module uses theme_menu_link, Superfish does not, this is why we have to do this.
            if (function_exists('context_active_contexts')) {
                if ($contexts = context_active_contexts()) {
                    foreach ($contexts as $context) {
                        if ((isset($context->reactions['menu']))) {
                            if ($menu_item['link']['href'] == $context->reactions['menu']) {
                                $menu_item['link']['localized_options']['attributes']['class'][] = 'active';
                            }
                        }
                    }
                }
            }

            $link_options += $menu_item['link']['localized_options'];

            // Render the menu item.
            // Should a theme be used for menu items?
            if ($settings['use_item_theme']) {
                $item_variables = array(
                    'element' => array(
                        'attributes' => array(
                            'id' => 'menu-' . $mlid . '-' . $id,
                            'class' => trim($item_class),
                        ),
                        'below' => ($show_children) ? $children['content'] : NULL,
                        'item' => $menu_item,
                        'localized_options' => $link_options,
                    ),
                    'properties' => array(
                        'megamenu' => array(
                            'megamenu_column' => $megamenu_column,
                            'megamenu_wrapper' => $megamenu_wrapper,
                            'megamenu_content' => $megamenu_content,
                        ),
                        'use_link_theme' => $settings['use_link_theme'],
                        'wrapper' => $whl,
                    ),
                );
                $output['content'] .= theme('superfish_menu_item', $item_variables);
            }
            else {
                $output['content'] .= '<li id="menu-' . $mlid . '-' . $id . '"';
                $output['content'] .= ($item_class) ? ' class="' . trim($item_class) . '">' : '>';
                $output['content'] .= ($megamenu_column) ? '<div class="sf-megamenu-column">' : '';
                $output['content'] .= isset($whl[0]) ? $whl[0] : '';
                if ($settings['use_link_theme']) {
                    $link_variables = array(
                        'menu_item' => $menu_item,
                        'link_options' => $link_options,
                    );
                    $output['content'] .= theme('superfish_menu_item_link', $link_variables);
                }
                else {
                    $output['content'] .= l($menu_item['link']['title'], $menu_item['link']['href'], $link_options);
                }
                $output['content'] .= isset($whl[1]) ? $whl[1] : '';
                $output['content'] .= ($megamenu_wrapper) ? '<ul class="sf-megamenu"><li class="sf-megamenu-wrapper ' . $item_class . '">' : '';
                $output['content'] .= ($show_children) ? (isset($wul[0]) ? $wul[0] : '') : '';
                $output['content'] .= ($show_children) ? (($megamenu_content) ? '<ol>' : '<ul>') : '';
                $output['content'] .= ($show_children) ? $children['content'] : '';
                $output['content'] .= ($show_children) ? (($megamenu_content) ? '</ol>' : '</ul>') : '';
                $output['content'] .= ($show_children) ? (isset($wul[1]) ? $wul[1] : '') : '';
                $output['content'] .= ($megamenu_wrapper) ? '</li></ul>' : '';
                $output['content'] .= ($megamenu_column) ? '</div>' : '';
                $output['content'] .= '</li>';
            }
        }
    }
    $output['total_children'] = $total_children;
    $output['parent_children'] = $parent_children;
    $output['single_children'] = $single_children;
    return $output;
}

function newmedialab_preprocess_region(&$variables) {
    $variables['leftlighted'] = block_get_blocks_by_region('leftlighted');
}

function newmedialab_form_alter(&$form, &$form_state, $form_id){
    if ($form_id == 'search_block_form') {
        $form['actions']['submit']['#value'] = t(html_entity_decode('&#xf002;')); // Change the text on the submit button
    }

    if($form_id == "user_login") {
        $form['name']['#description'] = 'Enter your username';
    }
}


function newmedialab_preprocess_page(&$variables){
    if(drupal_is_front_page()){
        $variables['title']="";
    }
    drupal_add_library('system', 'ui');
    drupal_add_library('system', 'ui.accordion');
}

function newmedialab_preprocess_search_result(&$variables) {
  $variables['info'] = '';
} 
