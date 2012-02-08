DRUPAL SPLASHIFY MODULE
------------------------
Maintainers:
  Chris Roane (http://drupal.org/user/1283000)
Requires - Drupal 7 and jStorage (jQuery plugin)
License - GPL (see LICENSE)


OVERVIEW:
---------
Splashify is a full featured splash page module that is designed to be search
engine friendly. It is originally based on the Drupal 6 Splash module. It
allows you to specify a page to be displayed anywhere on your site, using one
of a few different delivery options (redirect, popup window or ColorBox). The
module also provides options specific to mobile devices.

The main focus of this module is the following:
- Be search engine friendly by redirecting via JavaScript (when applicable).
- Allow specific options for mobile devices.
- Allow different ways in delivering the splash page.
- Use ColorBox for displaying the splash page as a lightbox.
- You can have the system pick from a random list of splash pages, have a
list of splash pages show up in a specified order, display the specified
text/html in the site template or display the text/html full screen.


INSTALLATION:
-------------
1. Download the jStorage jquery library and place the files in this folder:
   sites/all/libraries/jstorage/
   Link: http://www.jstorage.info/
2. Download and unpack the Splashify module directory in your modules folder
   (this will usually be "sites/all/modules/").
3. Go to "Administer" -> "Modules" and enable the module.

If you want to allow the lightbox functionality, you will need to install the
Colorbox module: http://drupal.org/project/colorbox .


CONFIGURATION:
--------------
Go to "Configuration" -> "System" -> "Splashify" to find all the configuration
options.


PROBLEMS OR FEATURE REQUESTS
-----------------------------
First make sure an issue doesn't already exist. If it doesn't, create a new
issue: http://drupal.org/node/add/project-issue/1423456


LAST UPDATED
-------------
02/08/2012
