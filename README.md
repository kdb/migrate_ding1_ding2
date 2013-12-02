Description
-----------
The module provides migration classes for performing migrations from a Ding/Drupal 6 site to a Ding2/Drupal 7 site.


Dependencies
------------

This module depends on the following modules:

* [http://drupal.org/project/migrate](migrate) (version 7.x-2.6-RC1 or above)
* [http://drupal.org/project/migrate_extras](migrate_extras) 
* [http://drupal.org/project/migrate_d2d](migrate_d2d) (version 7.x-2.1-beta1 or above)

Make sure to get the 2.6-RC1 version of migrate and 2.1-beta1 of migrate_d2d (or above), or migrate_ding1_ding2 will not run.

Installation
------------

Download and enable the module on an existing Ding2 installation. 

In settings.php, point to the ding1 site database you are migrating from. Add this entry to your settings.php file:

    $databases['legacy']['default'] = array (
      'database' => 'DATABASE_NAME',
      'username' => 'DATABASE_USER',
      'password' => 'PASSWORD',
      'host' => 'localhost',
      'port' => '',
      'driver' => 'mysql',
      'prefix' => '',
    );

Substituting your own connection info. 
"Legacy" is the default database which the migrate module will try to connect to.

Also, in settings.php, add this line, supplying your own absolute path where your ding1 site's files reside:
    $conf['migrate_ding1_ding2_source_dir'] = '/home/YOUR-DING1-SITE/sites/default/files';

If you plan to migrate users (and you probably would), make sure to transfer the variable drupal_private_key from your ding1 installation to the new ding-site. Using drush, you can navigate to your old ding1 site and get your drupal_private_key with:

    drush vget drupal_private_key
Now, navigate to your ding2 site, and insert the key with:

    drush vset drupal_private_key {KEY-VALUE}

Configuration of the module
---------------------------

No UI configuration. All configuration is in code only. Go to /admin/content/migrate to view the state of the migration process.


Resources
---------

Migrations are complex, and they are handled purely through code - the Migrate UI is only a convenient overview during the process. Resources I have consulted include:

* [http://www.acquia.com/blog/drupal-drupal-data-migration-part-1-basics](http://www.acquia.com/blog/drupal-drupal-data-migration-part-1-basics)
* [http://www.acquia.com/blog/drupal-drupal-data-migration-part-2-architecture](http://www.acquia.com/blog/drupal-drupal-data-migration-part-2-architecture)
* [http://drupal.stackexchange.com/questions/50004/file-or-image-migrate-from-drupal-6-to-drupal-7-using-migrate-d2d-module](http://drupal.stackexchange.com/questions/50004/file-or-image-migrate-from-drupal-6-to-drupal-7-using-migrate-d2d-module)
* [http://www.grasmash.com/article/migrate-classes-location-cck-address-field](http://www.grasmash.com/article/migrate-classes-location-cck-address-field)
* [http://btmash.com/article/2011-02-25/migrating-content-using-migrate-module](http://btmash.com/article/2011-02-25/migrating-content-using-migrate-module)
* [http://dtek.net/blog/drupal-6-to-drupal-7-via-migrate-2](http://dtek.net/blog/drupal-6-to-drupal-7-via-migrate-2)

