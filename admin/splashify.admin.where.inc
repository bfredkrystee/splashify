<?php

/**
 * @file
 * The admin "Where" tab.
 *
 * Admin functionality that determines where the splash page should show up
 * on the website.
 */

 /**
 * "Where" settings tab.
 */
function splashify_admin_where_form($form, &$form_state) {
  $form = array();

  $form['description'] = array(
    '#markup' => '<p>' . t('Where should the splash page come up?') . '</p>',
  );

  $form['desktop'] = array(
    '#type' => 'fieldset',
    '#title' => t('Desktop Settings'),
  );

  $page_options = array('home' => t('Front Page'), 'all' => t('All Pages'), 'list' => t('List Pages'));
  $form['desktop']['splashify_where_desktop_page'] = array(
    '#type' => 'select',
    '#title' => t('Specify where the splash page should show up:'),
    '#default_value' => variable_get('splashify_where_desktop_page', 'home'),
    '#options' => $page_options,
    '#description' => t('Define where the splash page should show up.'),
    '#ajax' => array(
      'callback' => 'ajax_splashify_where_desktop_page_callback',
      'wrapper' => 'where-desktop-page-value',
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );

  // Set a variable that is either defined by the selection from the ajax
  // radio buttons, or a previously saved value.
  if (isset($form_state['values']['splashify_where_desktop_page'])) {
    $where_desktop_list_set = $form_state['values']['splashify_where_desktop_page'];
  }
  else {
    $where_desktop_list_set = variable_get('splashify_where_desktop_page', '');
  }

  // If they specified the redirect option, we want to hide the window size
  // text field.
  if ($where_desktop_list_set != 'list'){
    $form['desktop']['splashify_where_desktop_listpages'] = array(
      '#type' => 'hidden',
      '#prefix' => '<div id="where-desktop-page-value"',
      '#suffix' => '</div>',
      '#value' => '',
    );
  }
  else {
    $form['desktop']['splashify_where_desktop_listpages'] = array(
      '#type' => 'textarea',
      '#prefix' => '<div id="where-desktop-page-value"',
      '#suffix' => '</div>',
      '#title' => t('List Pages'),
      '#default_value' => variable_get('splashify_where_desktop_listpages', ''),
      '#description' => t('Enter the paths of the pages where you want the splash to show up (one per line). Do not use the alias for the page. Example: node/12'),
    );
  }

  $form['mobile'] = array(
    '#type' => 'fieldset',
    '#title' => t('Mobile Settings'),
  );

  // If the mobile splash is enabled, display the mobile options.
  if (variable_get('splashify_when_mobile', 0) == 1) {
    $javascript = <<<EOD
    jQuery(document).ready(function(){
      if(jQuery("#edit-splashify-where-mobile-listpages").val() != '') {
        jQuery("#mobile_listpages").show();
      }

      jQuery("input[name='splashify_where_mobile_page']").change(function(){
        if(jQuery('#edit-splashify-where-mobile-page-list').is(':checked')){
          jQuery("#mobile_listpages").show();
        } else {
          jQuery("#mobile_listpages").hide();
          jQuery("#edit-splashify-where-mobile-listpages").val('');
        }
      });
    });
EOD;

    drupal_add_js($javascript, array('type' => 'inline'));

    $form['mobile']['splashify_where_mobile_page'] = array(
      '#type' => 'radios',
      '#title' => t('Specify where the splash page should show up:'),
      '#default_value' => variable_get('splashify_where_mobile_page', 'home'),
      '#options' => $page_options,
    );

    $form['mobile']['splashify_where_mobile_listpages'] = array(
      '#prefix' => '<div id="mobile_listpages" style="display:none">',
      '#suffix' => '</div>',
      '#type' => 'textarea',
      '#title' => t('List Pages'),
      '#default_value' => variable_get('splashify_where_mobile_listpages', ''),
      '#description' => t('Enter the paths of the pages where you want the splash to show up (one per line). Do not use the alias for the page. Example: node/12'),
    );
  }
  else {
    $form['mobile']['splashify_what_mobile_mode'] = array(
      '#markup' => '<p>' . t('In order to specify mobile options, you need to enable the "When: Enable Unique Mobile Splash" option.') . '</p>',
    );
  }

  return system_settings_form($form);
}

/**
 * Ajax callback for the "which page" radio buttons.
 */
function ajax_splashify_where_desktop_page_callback($form, &$form_state) {
  return $form['desktop']['splashify_where_desktop_listpages'];
}
