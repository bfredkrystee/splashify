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
      'effect' => 'slide',
    ),
  );

  // Set a variable that is either defined by the selection from the ajax
  // dropdown, or a previously saved value.
  if (isset($form_state['values']['splashify_where_desktop_page'])) {
    $where_desktop_list_set = $form_state['values']['splashify_where_desktop_page'];
  }
  else {
    $where_desktop_list_set = variable_get('splashify_where_desktop_page', '');
  }

  $form['desktop']['options']['begin'] = array('#markup' => '<div id="where-desktop-page-value">', );

  // If they specified the redirect option, we want to hide the window size
  // text field.
  if ($where_desktop_list_set == 'list'){
    $form['desktop']['options']['splashify_where_desktop_listpages'] = array(
      '#type' => 'textarea',
      '#title' => t('List Pages'),
      '#default_value' => variable_get('splashify_where_desktop_listpages', ''),
      '#description' => t('Enter the paths of the pages where you want the splash to show up (one per line). Do not use the alias for the page. Example: node/12'),
    );
  }

  $form['desktop']['options']['end'] = array('#markup' => '</div>', );

  $form['mobile'] = array(
    '#type' => 'fieldset',
    '#title' => t('Mobile Settings'),
  );

  // If the mobile splash is enabled, display the mobile options.
  if (variable_get('splashify_when_mobile', 0) == 1) {
    $form['mobile']['splashify_where_mobile_page'] = array(
      '#type' => 'select',
      '#title' => t('Specify where the splash page should show up:'),
      '#default_value' => variable_get('splashify_where_mobile_page', 'home'),
      '#options' => $page_options,
      '#ajax' => array(
        'callback' => 'ajax_splashify_where_mobile_page_callback',
        'wrapper' => 'where-mobile-page-value',
        'method' => 'replace',
        'effect' => 'slide',
      ),
    );

    // Set a variable that is either defined by the selection from the ajax
    // drop down, or a previously saved value.
    if (isset($form_state['values']['splashify_where_mobile_page'])) {
      $where_mobile_list_set = $form_state['values']['splashify_where_mobile_page'];
    }
    else {
      $where_mobile_list_set = variable_get('splashify_where_mobile_page', '');
    }

    $form['mobile']['options']['begin'] = array('#markup' => '<div id="where-mobile-page-value">', );

    if ($where_mobile_list_set == 'list'){
      $form['mobile']['options']['splashify_where_mobile_listpages'] = array(
        '#type' => 'textarea',
        '#title' => t('List Pages'),
        '#default_value' => variable_get('splashify_where_mobile_listpages', ''),
        '#description' => t('Enter the paths of the pages where you want the splash to show up (one per line). Do not use the alias for the page. Example: node/12'),
      );
    }

    $form['mobile']['options']['end'] = array('#markup' => '</div>', );
  }
  else {
    $form['mobile']['splashify_what_mobile_mode'] = array(
      '#markup' => '<p>' . t('In order to specify mobile options, you need to enable the "When: Enable Unique Mobile Splash" option.') . '</p>',
    );
  }

  $form['#submit'][] = 'splashify_admin_where_form_submit';
  return system_settings_form($form);
}

/**
 * Implements hook_form_submit().
 */
function splashify_admin_where_form_submit($form, &$form_state) {
  if ($form_state['values']['splashify_where_desktop_page']  != 'list') {
    // Unset the listpages variable (desktop), if the list pages option is not set.
    variable_set('splashify_where_desktop_listpages', '');
  }

  if ($form_state['values']['splashify_where_mobile_page']  != 'list') {
    // Unset the listpages variable (mobile), if the list pages option is not set.
    variable_set('splashify_where_mobile_listpages', '');
  }
}

/**
 * Ajax callback for the "which page" select for desktop.
 */
function ajax_splashify_where_desktop_page_callback($form, &$form_state) {
  return $form['desktop']['options'];
}

/**
 * Ajax callback for the "which page" select for mobile devices.
 */
function ajax_splashify_where_mobile_page_callback($form, &$form_state) {
  return $form['mobile']['options'];
}
