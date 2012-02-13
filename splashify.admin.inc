<?php

/**
 * @file
 * The admin area for splashify.
 *
 * Each function in this file manages a display for a tab in the splashify
 * admin area. Most of these pages are just forms that use
 * system_settings_form() to store settings for splashify.
 */

/**
 * "Splashify" settings tab.
 */
function splashify_admin_main() {
  $output = '';
  $output .= '<p>' . t('Take a look at the !splashify_help. You can also look at the !splashify_project to see a list of features.', array(
    '!splashify_help' => l(t('Splashify Help Page'), 'admin/help/splashify'),
    '!splashify_project' => l(t('Splashify Project Page'),
      'http://drupal.org/sandbox/chrisroane/1423456', array(
        'attributes' => array(
          'target' => '_blank',
        ),
      )
    ),
  )) . '</p>';

  $output .= '<p>' . t('Use the tabs above to access the settings for this module.') . '</p>';

  return $output;
}

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

/**
 * "Where" settings tab.
 */
function splashify_admin_where_form() {
  $form = array();

  // Add Javascript to handle the List Pages field.
  $js_file = drupal_get_path('module', 'splashify') . '/js/spotify_admin_where.js';
  drupal_add_js($js_file, 'file');

  $form['description'] = array(
    '#markup' => '<p>' . t('Where should the splash page come up?') . '</p>',
  );

  $form['desktop'] = array(
    '#type' => 'fieldset',
    '#title' => t('Desktop Settings'),
  );

  $page_options = array('home' => t('Front Page'), 'all' => t('All Pages'), 'list' => t('List Pages'));
  $form['desktop']['splashify_where_desktop_page'] = array(
    '#type' => 'radios',
    '#title' => t('Specify where the splash page should show up:'),
    '#default_value' => variable_get('splashify_where_desktop_page', 'home'),
    '#options' => $page_options,
  );

  $form['desktop']['splashify_where_desktop_listpages'] = array(
    '#prefix' => '<div id="desktop_listpages" style="display:none">',
    '#suffix' => '</div>',
    '#type' => 'textarea',
    '#title' => t('List Pages'),
    '#default_value' => variable_get('splashify_where_desktop_listpages', ''),
    '#description' => t('Enter the paths of the pages where you want the splash to show up (one per line). Do not use the alias for the page. Example: node/12'),
  );

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
 * "What" settings tab.
 */
function splashify_admin_what_form() {
  $form = array();

  $form['description'] = array(
    '#markup' => '<p>' . t('What should show up for the splash page?') . '</p>',
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

/**
 * "How" settings tab.
 */
function splashify_admin_how_form() {
  $form = array();

  $form['description'] = array(
    '#markup' => '<p>' . t('How should the splash page come up?') . '</p>',
  );

  // Specify the display mode options for the splash page.
  $splashify_how_mode_options = array(
    'redirect' => t('Redirect'),
    'window' => t('Open in new window'),
  );

  if (module_exists('colorbox')) {
    $splashify_how_mode_options['lightbox'] = t('Open in a Lightbox (colorbox)');
  }
  else {
    $colorbox_warning = t('In order to access the lightbox option, you need to have the !colorbox module installed.', array(
      '!colorbox' => l(t('Colorbox'),
        'http://drupal.org/project/colorbox', array(
          'attributes' => array(
            'target' => '_blank',
          ),
        )
      ),
    ));

    drupal_set_message($colorbox_warning, 'warning');
  }

  $form['desktop'] = array(
    '#type' => 'fieldset',
    '#title' => t('Desktop Settings'),
  );

  // Determines how the splash page shows up.
  $form['desktop']['splashify_how_desktop_mode'] = array(
    '#type' => 'select',
    '#title' => t('Splash Display Mode'),
    '#options' => $splashify_how_mode_options,
    '#default_value' => variable_get('splashify_how_desktop_mode', 'redirect'),
    '#description' => t('Determines how the splash page should show up. If you want to use the lightbox option, you need to have the Colorbox module installed.'),
  );

  if (variable_get('splashify_how_desktop_mode', '') == 'redirect') {
    // If they specified the redirect option, we want to hide the window size
    // text field.
    $form['desktop']['splashify_how_desktop_size'] = array(
      '#type' => 'hidden',
      '#value' => '',
    );
  }
  else {
    $form['desktop']['splashify_how_desktop_size'] = array(
      '#type' => 'textfield',
      '#title' => t('Window/Box size'),
      '#default_value' => variable_get('splashify_how_desktop_size', ''),
      '#description' => t('Size (<code>WIDTHxHEIGHT</code>, e.g. 400x300) of the Window or Lightbox.'),
    );
  }

  $form['mobile'] = array(
    '#type' => 'fieldset',
    '#title' => t('Mobile Settings'),
  );

  // If the mobile splash is enabled, display the mobile options.
  if (variable_get('splashify_when_mobile', 0) == 1) {
    // Determines how the splash page shows up.
    $form['mobile']['splashify_how_mobile_mode'] = array(
      '#type' => 'select',
      '#title' => t('Splash Display Mode'),
      '#options' => $splashify_how_mode_options,
      '#default_value' => variable_get('splashify_how_mobile_mode', 'redirect'),
      '#description' => t('How should we load the splash page? Redirect, open in a new window or open in a lightbox. The disabled option prevents the splash from appearing.'),
    );

    if (variable_get('splashify_how_mobile_mode', '') == 'redirect') {
      // If they specified the redirect option, we want to hide the window
      // size text field.
      $form['mobile']['splashify_how_mobile_size'] = array(
        '#type' => 'hidden',
        '#value' => '',
      );
    }
    else {
      $form['mobile']['splashify_how_mobile_size'] = array(
        '#type' => 'textfield',
        '#title' => t('Window/Box size'),
        '#default_value' => variable_get('splashify_how_mobile_size', ''),
        '#description' => t('Size (<code>WIDTHxHEIGHT</code>, e.g. 400x300) of the Window or Lightbox.'),
      );
    }
  }
  else {
    $form['mobile']['splashify_how_mobile_mode'] = array(
      '#markup' => '<p>' . t('In order to specify mobile options, you need to enable the "When: Enable Unique Mobile Splash" option.') . '</p>',
    );
  }

  return system_settings_form($form);
}
