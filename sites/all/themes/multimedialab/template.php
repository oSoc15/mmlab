<?php

/**
 * @file
 * Template overrides as well as (pre-)process and alter hooks for the
 * MultimediaLab theme.
 */

function multimedialab_form_alter(&$form, &$form_state, $form_id)
{
    if ($form_id == 'search_block_form') {
        //$form['search_block_form']['#title'] = t('Search'); // Change the text on the label element
        //$form['search_block_form']['#title_display'] = 'invisible'; // Toggle label visibilty
        //$form['search_block_form']['#size'] = 40;  // define size of the textfield
        $form['search_block_form']['#default_value'] = t(html_entity_decode('&#x1f50d;')); // Set a default value for the textfield
        $form['search_block_form']['#attributes']['onfocus'] = "{this.value = '';}";
        //$form['search_block_form']['#attributes']['onblur'] = "if (this.value == '') {this.value = html_entity_decode('&#x1f50d;');}";
        $form['actions']['submit']['#value'] = t('Go !'); // Change the text on the submit button
        //$form['actions']['submit'] = array('#type' => 'image_button', '#src' => base_path() . path_to_theme() . '/images/search-button.png');

    }
}
?>