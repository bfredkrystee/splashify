<?php

/**
 * @file
 * The admin "Splashify" tab.
 *
 * Handles the default tab that shows up in the admin.
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