<?php

/**
 * @file
 * Handles displaying the splash page.
 *
 * Either creates the JS for handling the splash page (redirect, lightbox or
 * popup) or displays the What: Content full screen or in the site design.
 */

/**
 * Implements hook_init().
 *
 * Generates the JS for the redirect, lightbox or popup window.
 */
function splashify_init() {
  global $base_url;

  // Make sure the jStorage library is installed.
  if (!file_exists('sites/all/libraries/jstorage/jstorage.min.js')) {
    $jstorage_error = t('In order for Splashify to work, the !jstorage library needs to be installed under: sites/all/libraries/jstorage/ .', array(
      '!jstorage' => l(t('jStorage'),
      'http://www.jstorage.info/', array(
        'attributes' => array(
          'target' => '_blank',
        ),
      )),
    ));

    drupal_set_message(filter_xss($jstorage_error), 'error');
    return;
  }

  // If this is the cron script or drush, do not run the splashify code.
  if ((function_exists('drush_verify_cli') && call_user_func('drush_verify_cli')) || (strpos($_SERVER['PHP_SELF'], 'cron.php') !== FALSE)) {
    return;
  }

  /*
   * Step #1: Retrieve the admin settings.
   */

  $config = _splashify_get_config();

  $when_frequency     = $config['when_frequency'];
  $where_page         = $config['where_page'];
  $where_listpages    = $config['where_listpages'];
  $what_mode          = $config['what_mode'];
  $what_content       = $config['what_content'];
  $what_content_value = $config['what_content_value'];
  $how_mode           = $config['how_mode'];
  $how_size           = $config['how_size'];


  /*
   * Step #2: Should we display the splash?
   */

  $splash_display = FALSE;
  $splash_reason = '';

  if (!isset($_GET['splash'])) {
    $_GET['splash'] = '';
  }

  /*
   * Determine if this page should show the splash page.
   */

  $splash_correct_page = FALSE;
  if ($where_page == 'all') {
    // Display on all pages.
    $splash_correct_page = TRUE;
  }
  elseif ($where_page == 'home') {
    // Display on the home page.
    if ($_GET['q'] == drupal_get_normal_path(variable_get('site_frontpage', 'node'))) {
      $splash_correct_page = TRUE;
    }
  }
  elseif ($where_page == 'list') {
    // Display the splash on the provided list of pages. Loop through the
    // paths.
    $list_paths = preg_split('/[\n\r]+/', $where_listpages);
    foreach ($list_paths as $list_path) {
      $list_path = trim($list_path);
      if ($list_path == $_GET['q']) {
        $splash_correct_page = TRUE;
        break;
      }
    }
  }

  /*
   * Goes through the main server side checks to see if we should show the
   * splash page.
   */

  if (empty($what_content_value)) {
    // The What: Content field is not specified.
    $splash_reason = 'No What: Content value.';
  }
  elseif ($_GET['splash'] == 'off') {
    // Special case for preventing the splash page from showing up.
    $splash_reason = 'Splash page forced not to be displayed.';
  }
  elseif ($_GET['splash'] == 'on') {
    // Special case for forcing the splash page to show up.
    $splash_display = TRUE;
    $splash_reason = 'Splash page forced to be displayed.';
  }
  elseif ($splash_correct_page) {
    // We need to show the splash page on this page.
    // Make sure we are not coming from an internal page.
    if (($parsed_url = parse_url($base_url)) && stristr($_SERVER['HTTP_REFERER'], $parsed_url['host'])) {
      $splash_reason = 'Correct page is loaded from an internal link.';
    }
    else {
      $splash_display = TRUE;
      $splash_reason = 'This is the correct page.';
    }
  }

  // If we shouldn't display the splash, end the code here.
  if (!$splash_display) {
    return;
  }


  /*
   * Step #3: Display the Splash Page
   *
   * At this point, we know that $splash_display == TRUE.
   */


  /*
   * Determine the time values.
   */


  // Controls when the jStorage variable should expire next. In other words,
  // when should the splash page show up again?
  $js_expiretime = '';

  // Since the time is based on server time, we need to pass this value to
  // the JS.
  $js_nowtime = time();

  // This variable is a way to always show the splash in the JS.
  $js_splash_always = 'false';

  // Once.
  if ($when_frequency == 'once') {
    // Set to expire in one year.
    $js_expiretime = time() + (365 * 24 * 60 * 60);
  }
  // Every Day.
  elseif ($when_frequency == 'daily') {
    // Set to expire in 24 hours.
    $js_expiretime = time() + (24 * 60 * 60);
  }
  // Next Week.
  elseif ($when_frequency == 'weekly') {
    // Set to expire in 7 days.
    $js_expiretime = time() + (7 * 24 * 60 * 60);
  }
  // Never Expire.
  elseif ($when_frequency == 'always' || $_GET['splash'] == 'on') {
    // This should make the splash always show up.
    $js_expiretime = time();
    $js_splash_always = 'true';
  }

  /*
   * If we are working with a url, we need to specify that url value here
   */

  $action_url = '';
  $js_before = '';

  if ($what_mode == 'template' || $what_mode == 'fullscreen') {

    // We need to redirect to the /splash page.
    // Define the action url.
    $js_before .= "var action_url = 'splash';";
  }
  elseif ($what_mode == 'sequence' || $what_mode == 'random') {

    // We are working with url values in the What: Content field.
    $paths = preg_split('/[\n\r]+/', $what_content_value);
    if ($what_mode == 'sequence') {

      /*
       * Since we are using jStorage to store the last splash page that was
       * displayed, we need to create an array of the sequential url's and
       * figure out which is the next url to display (based on the previous
       * url)...all through JS.
       */


      // Create the JS array of url values.
      $js_before .= 'var what_urls = [];';
      $path_count = 0;
      foreach ($paths as $path) {
        $path = url(drupal_strip_dangerous_protocols(trim($path)));
        $js_before .= "what_urls[{$path_count}] = '{$path}';";
        $path_count++;
      }

      // Default the new url index value to the first url.
      $js_before .= "var new_url_index = 0;";

      // Have we already loaded a splash page? If so, grab the index of
      // that url.
      $js_before .= "var last_url = (jQuery).jStorage.get('splashlasturl', '');";
      $js_before .= "var last_url_index = what_urls.indexOf(last_url);";

      // Make sure the last url index value is correct.
      // If so, set the new index url to the [last index url] + 1.
      $js_before .= "if(last_url_index > -1 && last_url_index+1 < {$path_count}) { new_url_index = last_url_index + 1; }";

      // Define the action url.
      $js_before .= "var action_url = what_urls[new_url_index];";

      // Set the splash url used for this page, so we can display the correct
      // splash on the next page load.
      $js_before .= "(jQuery).jStorage.set('splashlasturl', action_url);";
    }
    elseif ($what_mode == 'random') {

      $next_index = array_rand($paths);

      // Define the action url.
      $js_before = "var action_url = '" . url(drupal_strip_dangerous_protocols(trim($paths[$next_index]))) . "';";
    }
  }

  /*
   * Generate the JS code that should be executed when the splash page should
   * show up.
   */

  $size_action = $how_size ? explode('x', $how_size) : FALSE;
  $js_action = '';
  if ($how_mode == 'redirect') {

    // Redirect to a different url.
    $js_action = "{$js_before} window.location = action_url;";
  }
  elseif ($how_mode == 'window') {

    // Open up a popup window.
    $size_str = $size_action ? ', "width=' . $size_action[0] . ',height=' . $size_action[1] . '"' : '';
    $js_action = "{$js_before} window.onload = function() { window.open(action_url, 'splash'{$size_str}); };";
  }
  elseif ($how_mode == 'lightbox' && module_exists('colorbox')) {

    // Open up a lightbox.
    $colorbox_settings = array();
    $colorbox_settings['transition'] = "transition:'elastic'";
    $colorbox_settings['iframe'] = "iframe:'true'";
    $colorbox_settings['href'] = 'href:action_url';

    // Set size.
    if (count($size_action) > 1) {
      $colorbox_settings['width'] = 'width:' . $size_action[0];
      $colorbox_settings['height'] = 'height:' . $size_action[1];
    }
    else {
      // The colorbox has strange behavior when we don't set the dimensions.
      $colorbox_settings['width'] = 'width:800';
      $colorbox_settings['height'] = 'height:600';
    }

    $colorbox_load_string = implode(', ', $colorbox_settings);

    // Generate the JS to load the colorbox after the page loads.
    $js_action = 'jQuery(document).ready(function(){ ' . $js_before . ' jQuery.colorbox({' . $colorbox_load_string . '}); });';
  }
  else {
    // Do nothing! This is a fail safe.
    return;
  }

  /*
   * Finally: Generate the JS that puts it all together!
   *
   * At this point the page passed all of the server side checks. We now
   * implement JS code that checks if the splash page should show up, based
   * on when it last showed up.
   */

  $splash_js = <<<EOD
    var splash = (jQuery).jStorage.get("splash");
    var nowtimeSeconds = {$js_nowtime};
    if(!splash || splash < nowtimeSeconds || {$js_splash_always}){
      // Set when the splash variable should expire.
      (jQuery).jStorage.set("splash", {$js_expiretime});

      // Do the splash action.
      {$js_action}
    }
EOD;

  drupal_add_js('sites/all/libraries/jstorage/jstorage.min.js', array(
      'type' => 'file',
      'scope' => 'header',
      'group' => JS_LIBRARY,
      'weight' => 0,
      'cache' => FALSE,
    ));

  drupal_add_js($splash_js, array(
      'type' => 'inline',
      'scope' => 'header',
      'group' => JS_LIBRARY,
      'weight' => 1,
      'cache' => FALSE,
    ));
}

/**
 * Display the What: Content either in the site template or full screen.
 */
function splashify_display_splashtext() {
  $config = _splashify_get_config();

  $when_frequency     = $config['when_frequency'];
  $where_page         = $config['where_page'];
  $where_listpages    = $config['where_listpages'];
  $what_mode          = $config['what_mode'];
  $what_content       = $config['what_content'];
  $what_content_value = $config['what_content_value'];
  $how_mode           = $config['how_mode'];
  $how_size           = $config['how_size'];

  if ($what_mode == 'template') {
    // Display what they entered in the field in the site template.
    return $what_content_value;
  }
  elseif ($what_mode == 'fullscreen') {
    // Display what they entered in the field as a full page.
    echo $what_content_value;
    exit;
  }
}

/**
 * Retrieves the admin settings.
 *
 * Will either retrieve the desktop or mobile settings, depending whether this
 * is a mobile request or not.
 */
function _splashify_get_config() {
  // This controls whether mobile is enabled or not.
  $is_mobile_enabled = variable_get('splashify_when_mobile', 0);

  // This controls whether or not we should use the mobile settings for this
  // request.
  $do_mobile = FALSE;

  // If this admin setting is true, force the system to use the mobile
  // settings.
  $when_mobile_test = variable_get('splashify_when_mobile_test', 0);
  $force_mobile = ($when_mobile_test == 0) ? FALSE : TRUE;

  if (!empty($is_mobile_enabled) && module_exists('mobile_tools')) {

    // Is this a mobile request?
    module_load_include('module', 'mobile_tools');
    $mobile = mobile_tools_is_mobile_device();
    if ($mobile['type'] != 'desktop' || $force_mobile) {
      // This is a mobile request!
      $do_mobile = TRUE;

      // Get the mobile admin settings.
      $when_frequency  = variable_get('splashify_when_mobile_frequency', 'never');
      $where_page      = variable_get('splashify_where_mobile_page', 'home');
      $where_listpages = variable_get('splashify_where_mobile_listpages', '');
      $what_mode       = variable_get('splashify_what_mobile_mode', 'random');
      $what_content    = variable_get('splashify_what_mobile_content', '');
      $how_mode        = variable_get('splashify_how_mobile_mode', 'redirect');
      $how_size        = variable_get('splashify_how_mobile_size', '');
    }
  }

  if (!$do_mobile) {
    // Get the desktop admin settings.
    $when_frequency  = variable_get('splashify_when_desktop_frequency', 'never');
    $where_page      = variable_get('splashify_where_desktop_page', 'home');
    $where_listpages = variable_get('splashify_where_desktop_listpages', '');
    $what_mode       = variable_get('splashify_what_desktop_mode', 'random');
    $what_content    = variable_get('splashify_what_desktop_content', '');
    $how_mode        = variable_get('splashify_how_desktop_mode', 'redirect');
    $how_size        = variable_get('splashify_how_desktop_size', '');
  }

  // $what_content is a text_format field. Get the value.
  $what_content_value = $what_content['value'];

  return array(
    'when_frequency' => $when_frequency,
    'when_mobile_test' => $when_mobile_test,
    'where_page' => $where_page,
    'where_listpages' => $where_listpages,
    'what_mode' => $what_mode,
    'what_content' => $what_content,
    'what_content_value' => $what_content_value,
    'how_mode' => $how_mode,
    'how_size' => $how_size,
  );
}
