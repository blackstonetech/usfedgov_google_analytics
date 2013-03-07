US Fed Gov Google Analytics Drupal Module
=========================================

Embeds appropriate GA javascript file for US federal agencies

RELEASE NOTES:

To change the agency js file, you must select the file in the admin gui accessible at /admin/config/system/usfedgov_google_analytics. The .js files in the fed_analytics libraries directory will dynamically populate the select list. You must follow the installation instructions below to download the library manually or via drush.

RELEASE CANDIDATE

Dependencies: Libraries API module (2.x)

MANUAL INSTALATION
==================
1. Enable the Libraries API module
2. Download the Federal Government Google Analytics files at https://github.com/GSA/DAP-Gov-wide-GA-Code/archive/master.zip
3. Extract the .zip file and rename it to fed_analytics
4. Place the fed_analytics file containing the js files into your libraries directory. If you do not have a libraries directory at sites/all/libraries (or sites/example.com/libraries for multisites), create it at /sites/all/libraries.
5. Enable the usfedgov_google_analytics module.

DRUSH INSTALLATION
==================
1. Enable the Libraries API module.
2. Enable the usfedgov_google_analytics_libs module.
3. Run drush fed_analytics_libs If the fed_analytics directory does not exist at /sites/all/libraries, it will be created.
4. Enable the usfedgov_google_analytics module.


Note: If you change the js file selection, you must clear your site's javascript cache to avoid including the previous cached js selection.

