<?php
/**
 * @file
 * Class for migrating files into Ding2.
 */

class DingFileMigration extends DrupalFile6Migration {
  public function __construct($arguments) {
    // in settings.php file, add the absolute path your ding1 sites files reside in by including the line:
    // $conf['migrate_ding1_ding2_source_dir'] = '/home/YOUR-DING1-SITE/sites/default/files';
    $source_dir = variable_get('migrate_ding1_ding2_source_dir', '/home/web/YOUR/SOURCE/FILES');

    // The description and the migration machine name are also required arguments,
    // which will be unique for each migration you register.
    $file_arguments = array(
      'source_connection' => 'legacy',
      'source_version' => 6,
      'description' => t('Migration of files from Ding1/D6'),
      'machine_name' => 'DingFile',
      'group_name' => 'ding1_group',
      'source_dir' => $source_dir,
      'user_migration' => 'DingUser',
    );

    parent::__construct($file_arguments);

    // Removing and redefining existing field mapping
    $this->removeFieldMapping('destination_file');
    $this->addFieldMapping('destination_file', 'filepath')
         ->description(t('filepath is trimmed and path modified to fit destination directories'))
         ->callbacks(array($this, 'get_modified_path'));
    // urlencode option, see https://drupal.org/node/1540106#urlencode
    $this->addFieldMapping('urlencode')
         ->description(t('urlencode set to TRUE has no effect on this migration, but we set it anyway'))
         ->defaultValue(1);

    // Unmapped destination fields
    $this->addUnmigratedDestinations(array('path'));
  }

  //Function to strip "sites/default/files/" and modify path based on strings in filepath
  protected function get_modified_path($value) {
    $file_directory_path = variable_get('migrate_ding1_ding2_file_directory_path', 'sites/default/files');
    $value = str_replace($file_directory_path . '/', '', $value);

    // Get path part
    $path_value = substr($value, 0, strpos($value,'/'));

    // Change path part in filepath according to a list
    switch ($path_value) {
      case "campaigns":
        $value = str_replace('campaigns/', 'ding_campaign_image/', $value);
        break;
      case "content_images":
        $value = str_replace('content_images/', '', $value);
        break;
      case "file_attachments":
        $value = str_replace('file_attachments/', 'files/', $value);
        break;
      case "list_images":
        $value = str_replace('list_images/', 'list_image/', $value);
        break;
      case "profile_image":
        $value = str_replace('profile_image/', 'profile_image/staff/', $value);
        break;
      default:
        // modify nothing
        break;
    }

    return $value;
  }

}