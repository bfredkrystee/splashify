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
  $output .= '<p>' . t('Take a look at the <a href="@splashify_help">Splashify Help Page</a>. You can also look at the <a href="@splashify_project" target="_blank">Splashify Project Page</a> to see a list of features.', array(
    '@splashify_help' => '/admin/help/splashify',
    '@splashify_project' => 'http://drupal.org/sandbox/chrisroane/1423456',
  )) . '</p>';

  $output .= '<p>' . t('Use the tabs above to access the settings for this module.') . '</p>';

  return $output;
}
