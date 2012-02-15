<?php

/**
 * @file
 * The admin "When" tab.
 *
 * Admin functionality that determines when the splash page should show up
 * on the website.
 */

/**
 * "When" settings tab.
 */
function splashify_admin_when_form() {
  $form = array();

  $form['description'] = array(
    '#markup' => '<p>' . t('When should the splash page show up?') . '</p>',
  );

  $form['desktop'] = array(
    '#type' => 'fieldset',
    '#title' => t('Desktop Settings'),
  );

  $form['desktop']['splashify_when_desktop_frequency'] = array(
    '#type' => 'select',
    '#title' => t('How often should visitors see the splash page?'),
    '#default_value' => variable_get('splashify_when_desktop_frequency', 'never'),
    '#options' => array(
      'never' => t('Never (off)'),
      'always' => t('Always'),
      'once' => t('Once'),
      'daily' => t('Daily'),
      'weekly' => t('Weekly'),
    ),
  );

  $form['mobile'] = array(
    '#type' => 'fieldset',
    '#title' => t('Mobile Settings'),
  );

  $disable_mobile = TRUE;
  if (module_exists('mobile_tools')) {
    $disable_mobile = FALSE;
  }

  $mobiletools_link = t('In order to use this feature, you need to have the !mobiltools module enabled.', array(
    '!mobiltools' => l(t('Mobile Tools'),
      'http://drupal.org/project/mobile_tools', array(
        'attributes' => array(
          'target' => '_blank',
        ),
      )
    ),
  ));

  $form['mobile']['splashify_when_mobile'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Unique Mobile Splash'),
    '#default_value' => variable_get('splashify_when_mobile', 0),
    '#description' => $mobiletools_link,
    '#disabled' => $disable_mobile,
  );

  // If the mobile splash is enabled, display the mobile options.
  if (variable_get('splashify_when_mobile', 0) == 1) {
    $form['mobile']['splashify_when_mobile_frequency'] = array(
      '#type' => 'select',
      '#title' => t('How often should mobile visitors see the mobile splash page?'),
      '#default_value' => variable_get('splashify_when_mobile_frequency', 'never'),
      '#options' => array(
        'never' => t('Never (off)'),
        'always' => t('Always'),
        'once' => t('Once'),
        'daily' => t('Daily'),
        'weekly' => t('Weekly'),
      ),
    );

    $form['mobile']['splashify_when_mobile_test'] = array(
      '#type' => 'checkbox',
      '#title' => 'Test Mobile',
      '#default_value' => variable_get('splashify_when_mobile_test', 0),
      '#description' => t('Turn this option on to force the mobile settings to take affect so you can test from your desktop browser.'),
      '#disabled' => $disable_mobile,
    );
  }

  return system_settings_form($form);
}
