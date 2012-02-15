<?php

/**
 * @file
 * The admin "What" tab.
 *
 * Admin functionality that determines what should show up as the splash
 * page.
 */

/**
 * "What" settings tab.
 */
function splashify_admin_what_form() {
  $form = array();

  $form['description'] = array(
    '#markup' => '<p>' . t('What should show up as the splash page?') . '</p>',
  );

  $form['desktop'] = array(
    '#type' => 'fieldset',
    '#title' => t('Desktop Settings'),
  );

  $form['desktop']['splashify_what_desktop_mode'] = array(
    '#type' => 'select',
    '#title' => t('Splash Mode'),
    '#options' => array(
      'random' => t('Pick random path or URL from the list'),
      'sequence' => t('Pick next path or URL from the list'),
      'template' => t('Display entered text in the site template'),
      'fullscreen' => t('Display entered text/HTML full screen'),
    ),
    '#default_value' => variable_get('splashify_what_desktop_mode', 'random'),
    '#description' => t('Determines how the content field below will be used with the splash page.'),
  );

  $splashify_what_desktop_content = variable_get('splashify_what_desktop_content', '');
  $form['desktop']['splashify_what_desktop_content'] = array(
    '#type' => 'text_format',
    '#title' => t('Content'),
    '#default_value' => (is_array($splashify_what_desktop_content)) ? $splashify_what_desktop_content['value'] : '',
    '#format' => (is_array($splashify_what_desktop_content)) ? $splashify_what_desktop_content['format'] : '',
    '#description' => t('Text to show or paths/URLs (one on each line) to use.  To open a non-Drupal path, use an absolute URL, i.e. http://example.com/splash.html'),
  );

  $form['mobile'] = array(
    '#type' => 'fieldset',
    '#title' => t('Mobile Settings'),
  );

  // If the mobile splash is enabled, display the mobile options.
  if (variable_get('splashify_when_mobile', 0) == 1) {
    $form['mobile']['splashify_what_mobile_mode'] = array(
      '#type' => 'select',
      '#title' => t('Splash Mode'),
      '#options' => array(
        'random' => t('Pick random path or URL from the list'),
        'sequence' => t('Pick next path or URL from the list'),
        'template' => t('Display entered text in the site template'),
        'fullscreen' => t('Display entered text/HTML full screen'),
      ),
      '#default_value' => variable_get('splashify_what_mobile_mode', 'random'),
      '#description' => t('Determines how the content field below will be used with the splash page.'),
    );

    $splashify_what_mobile_content = variable_get('splashify_what_mobile_content', '');
    $form['mobile']['splashify_what_mobile_content'] = array(
      '#type' => 'text_format',
      '#title' => t('Content'),
      '#default_value' => (is_array($splashify_what_mobile_content)) ? $splashify_what_mobile_content['value'] : '',
      '#format' => (is_array($splashify_what_mobile_content)) ? $splashify_what_mobile_content['format'] : '',
      '#description' => t('Text to show or paths/URLs (one on each line) to use.  To open a non-Drupal path, use an absolute URL, i.e. http://example.com/splash.html'),
    );
  }
  else {
    $form['mobile']['splashify_what_mobile_mode'] = array(
      '#markup' => '<p>' . t('In order to specify mobile options, you need to enable the "When: Enable Unique Mobile Splash" option.') . '</p>',
    );
  }

  return system_settings_form($form);
}
