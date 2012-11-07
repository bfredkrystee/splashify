(function ($) {

  Drupal.behaviors.splashify = {
    attach: function (context, settings){
      $(window).load(function(){
        var show = 1;
        var referrer = document.referrer + '';
        var hostname = window.location.hostname + '';
        if (referrer.search(hostname) != -1) {
          // This page was loaded from an internal page. Do not show splash.
          show = 0;
        }
        var splash = $.jStorage.get("splash");
        var expireAfter = settings.splashify.js_expire_after;
        var splashalways = settings.splashify.js_splash_always;
        var now = new Date();
        var nowtimeSeconds = now.getTime() / 1000;

        // Display the splash page?
        if(show == 1 && (!splash || splash < nowtimeSeconds || splashalways=='1')){
          // Set when the splash variable should expire.
          $.jStorage.set("splash", nowtimeSeconds + expireAfter);

          var url = '';
          var last_url = $.jStorage.get('splashlasturl', '');
          var what_urls = settings.splashify.js_mode_settings.urls;

          // Determine the url we are working with, which is based on the mode.
          if(settings.splashify.js_mode_settings.system_splash != ''){
            // Display the system splash page.
            url = settings.splashify.js_mode_settings.system_splash;
          } else if(settings.splashify.js_mode_settings.mode == 'sequence'){
            // Display the splash pages in sequence.
            var new_url_index = 0;
            var last_url_index = jQuery.inArray(last_url, what_urls);
            if(last_url_index > -1 && last_url_index+1 < settings.splashify.js_mode_settings.total_urls){
              new_url_index = last_url_index + 1;
            }
            url = what_urls[new_url_index];
          } else if(settings.splashify.js_mode_settings.mode == 'random'){
            // Display a random splash page.
            var new_url_index = Math.floor(Math.random() * settings.splashify.js_mode_settings.total_urls);
            url = what_urls[new_url_index];
          }

          $.jStorage.set('splashlasturl', url);

          // Display the splash page.
          if(settings.splashify.js_mode == 'colorbox'){
            // Open a ColorBox.
            $.colorbox({
              transition:'elastic',
              iframe:settings.splashify.js_mode_settings.iframe_option,
              href:url,
              width:settings.splashify.js_mode_settings.size_width,
              height:settings.splashify.js_mode_settings.size_height
            });
          } else if(settings.splashify.js_mode == 'redirect'){
            // Redirect.
            window.location.replace(url);
          } else if(settings.splashify.js_mode == 'window'){
            // Open a popup window.
            window.open(url, 'splash', settings.splashify.js_mode_settings.size);
          }
        }
      });
    }
  };

})(jQuery);
