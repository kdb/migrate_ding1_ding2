<?php
/**
 * @file
 * Classes for migrating into Ding2/D7 Profile2 entities.
 */
class DingProfileProviderAlmaMigration extends Migration {
  public function __construct($arguments) {
    global $user;

    $this->team = array(
      new MigrateTeamMember('Ewan Andreasen', 'ea@vejlebib.dk', t('Webdeveloper (Ding1 -> Ding2 Migration)')),
    );
    $this->description = t('Migration of user provider data from Ding1/D6');
    $this->dependencies = array('DingUser', 'DingFile');

    $arguments = array(
      'machine_name' => 'DingProfileProviderAlma',
      'group_name' => 'ding1_group',
    );

    // Add to sourceFields before calling parent constructor.
    // See http://www.acquia.com/blog/drupal-drupal-data-migration-part-2-architecture
    $this->sourceFields['interest_period'] = t('Interest period extracted from field data');

    parent::__construct($arguments);

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'uid' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
        )
      ),
      MigrateDestinationProfile2::getKeySchema()
    );

    // Create a MigrateSource object, which manages retrieving the input data.
    $this->source = new MigrateSourceSQL($this->query(), $this->sourceFields, NULL, array('map_joinable' => FALSE));

    $this->destination = new MigrateDestinationProfile2('provider_alma');

    //map fields
    $this->addFieldMapping('uid', 'uid')
         ->sourceMigration('DingUser')
         ->description(t('The assignment of profile2-items to the respective user'));
    $this->addFieldMapping('revision_uid')
      ->defaultValue($user->uid)
      ->description(t('The user ID of the user, who started the migration'));
    $this->addFieldMapping('language')
      ->defaultValue('und');
    $this->addFieldMapping('field_alma_interest_period', 'interest_period');
    $this->addFieldMapping(NULL, 'data')
         ->description(t('Handled in prepareRow function'));

    // Unmapped destination fields
    $this->addFieldMapping('field_alma_absent_id')
      ->description(t('Not mapped - User data displayed directly through Alma Provider'))
      ->issueGroup(t('DNM'));
    $this->addFieldMapping('field_alma_absent_id:language')
      ->description(t('Not mapped - User data displayed directly through Alma Provider'))
      ->issueGroup(t('DNM'));
    $this->addFieldMapping('field_alma_city')
      ->description(t('Not mapped - User data displayed directly through Alma Provider'))
      ->issueGroup(t('DNM'));
    $this->addFieldMapping('field_alma_city:language')
      ->description(t('Not mapped - User data displayed directly through Alma Provider'))
      ->issueGroup(t('DNM'));
    $this->addFieldMapping('field_alma_full_name')
      ->description(t('Not mapped - User data displayed directly through Alma Provider'))
      ->issueGroup(t('DNM'));
    $this->addFieldMapping('field_alma_full_name:language')
      ->description(t('Not mapped - User data displayed directly through Alma Provider'))
      ->issueGroup(t('DNM'));
    $this->addFieldMapping('field_alma_preferred_branch')
      ->description(t('Not mapped - User data displayed directly through Alma Provider'))
      ->issueGroup(t('DNM'));
    $this->addFieldMapping('field_alma_mail')
      ->description(t('Not mapped - User data displayed directly through Alma Provider'))
      ->issueGroup(t('DNM'));
    $this->addFieldMapping('field_alma_mobile_phone')
      ->description(t('Not mapped - User data displayed directly through Alma Provider'))
      ->issueGroup(t('DNM'));
    $this->addFieldMapping('field_alma_mobile_phone:language')
      ->description(t('Not mapped - User data displayed directly through Alma Provider'))
      ->issueGroup(t('DNM'));
    $this->addFieldMapping('field_alma_phone_id')
      ->description(t('Not mapped - User data displayed directly through Alma Provider'))
      ->issueGroup(t('DNM'));
    $this->addFieldMapping('field_alma_phone_id:language')
      ->description(t('Not mapped - User data displayed directly through Alma Provider'))
      ->issueGroup(t('DNM'));
    $this->addFieldMapping('field_alma_postal_code')
      ->description(t('Not mapped - User data displayed directly through Alma Provider'))
      ->issueGroup(t('DNM'));
    $this->addFieldMapping('field_alma_postal_code:language')
      ->description(t('Not mapped - User data displayed directly through Alma Provider'))
      ->issueGroup(t('DNM'));
    $this->addFieldMapping('field_alma_reservation_pause')
      ->description(t('Not mapped - User data displayed directly through Alma Provider'))
      ->issueGroup(t('DNM'));
    $this->addFieldMapping('field_alma_reservation_pause:value2')
      ->description(t('Not mapped - User data displayed directly through Alma Provider'))
      ->issueGroup(t('DNM'));
    $this->addFieldMapping('field_alma_street_name')
      ->description(t('Not mapped - User data displayed directly through Alma Provider'))
      ->issueGroup(t('DNM'));
    $this->addFieldMapping('field_alma_street_name:language')
      ->description(t('Not mapped - User data displayed directly through Alma Provider'))
      ->issueGroup(t('DNM'));
    $this->addFieldMapping('path')
      ->issueGroup(t('DNM'));
  }

  public function prepareRow($row) {
    // Always include this snippet, in case our parent class decides to ignore the row
    if (parent::prepareRow($row) === FALSE) {
       return FALSE;
    }

    // Get values from serialized data and populate the source fields defined in our constructor
    $user_data = unserialize($row->data);
    $row->interest_period = $user_data['interest_period'];
  }

  // Modify the query to select only alma users
  protected function query() {
    $query = Database::getConnection('default', 'legacy')
             ->select('users', 'u')
             ->fields('u', array('uid', 'data'));
    $query->condition('name','%@alma_user', 'LIKE');
    return $query;
  }
}


class DingProfileStaffMigration extends Migration {
  public function __construct($arguments) {
    global $user;

    $this->team = array(
      new MigrateTeamMember('Ewan Andreasen', 'ea@vejlebib.dk', t('Webdeveloper (Ding1 -> Ding2 Migration)')),
    );
    $this->description = t('Migration of user content profile data from Ding1/D6');
    $this->dependencies = array('DingUser', 'DingFile', 'DingLibrary');

    $arguments = array(
      'machine_name' => 'DingProfileStaff',
      'group_name' => 'ding1_group',
    );

    // Add to sourceFields before calling parent constructor.
    // See http://www.acquia.com/blog/drupal-drupal-data-migration-part-2-architecture
    $this->sourceFields['firstname_extracted'] = t('Firstname extracted from nr_title');
    $this->sourceFields['surname_extracted'] = t('Surname extracted from nr_title');
    $this->sourceFields['profile_image_alt'] = t('Alt text extracted from field_image_data');
    $this->sourceFields['profile_image_title'] = t('Title text extracted from field_image_data');
    $this->sourceFields['departments'] = t('table lookup in prepareRow function');
    $this->sourceFields['libraries'] = t('table lookup in prepareRow function');

    parent::__construct($arguments);

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'nid' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => TRUE,
          'description' => 'D6 Unique Node ID',
          'alias' => 'n',
        )
      ),
      MigrateDestinationProfile2::getKeySchema()
    );

    // Create a MigrateSource object, which manages retrieving the input data.
    $this->source = new MigrateSourceSQL($this->query(), $this->sourceFields, NULL, array('map_joinable' => FALSE));

    $this->destination = new MigrateDestinationProfile2('ding_staff_profile');

    //map fields
    $this->addFieldMapping('uid', 'uid')
         ->sourceMigration('DingUser')
         ->description(t('The assignment of profile2-items to the respective user'));
    $this->addFieldMapping('revision_uid')
      ->defaultValue($user->uid)
      ->description(t('The user ID of the user, who started the migration'));
    $this->addFieldMapping('language')
      ->defaultValue('und');
    $this->addFieldMapping('field_ding_staff_description', 'body');
    $this->addFieldMapping('field_ding_staff_description:format')
         ->defaultValue('ding_wysiwyg');
    $this->addFieldMapping('field_ding_staff_description:language')
         ->defaultValue('und');
    $this->addFieldMapping('field_ding_staff_position', 'field_staff_title_value');
    $this->addFieldMapping('field_ding_staff_position:language')
         ->defaultValue('und');
    $this->addFieldMapping('field_ding_staff_forename', 'firstname_extracted');
    $this->addFieldMapping('field_ding_staff_forename:language')
         ->defaultValue('und');
    $this->addFieldMapping('field_ding_staff_surname', 'surname_extracted');
    $this->addFieldMapping('field_ding_staff_surname:language')
         ->defaultValue('und');
    $this->addFieldMapping('field_ding_staff_phone', 'field_phone_value');
    $this->addFieldMapping('field_ding_staff_phone:language')
         ->defaultValue('und');
    $this->addFieldMapping('field_ding_staff_email', 'mail');
    $this->addFieldMapping('field_ding_staff_department', 'departments')
         ->description(t('See prepareRow method'));
    // Even though default is termname, :source_type needs to be explicitly set
    // to something other than tid for the migration to work
    $this->addFieldMapping('field_ding_staff_department:source_type')
         ->defaultValue('name')
         ->description(t('Explicitly set to other value than -tid-, i.e. -name- in this case'));
    $this->addFieldMapping('field_ding_staff_department:create_term')
         ->defaultValue(TRUE);
    $this->addFieldMapping('field_ding_staff_department:ignore_case')
         ->defaultValue(TRUE);
    $this->addFieldMapping('og_group_ref', 'libraries')
         ->sourceMigration('DingLibrary')
         ->description(t('See prepareRow method'));

    // map profile image field
    $this->addFieldMapping('field_ding_staff_image', 'field_image_fid')
         ->sourceMigration('DingFile');
    $this->addFieldMapping('field_ding_staff_image:file_class')
         ->defaultValue('MigrateFileFid');
    $this->addFieldMapping('field_ding_staff_image:language')
         ->defaultValue('und');
    $this->addFieldMapping('field_ding_staff_image:preserve_files')
         ->defaultValue(TRUE);

    $this->addFieldMapping('field_ding_staff_image:alt', 'profile_image_alt');
    $this->addFieldMapping('field_ding_staff_image:title', 'profile_image_title');
    $this->addFieldMapping(NULL, 'field_image_data')
         ->description(t('Split this into profile_image_alt and profile_image_title for D7'));

    // Unmapped source fields
    $this->addUnmigratedSources(array(
      'teaser',
      'title',
      'field_work_area_value',
      'nr_title',
      'name',
      'field_surname_value',
      'field_firstname_value',
      'vid',
    ));

    // Unmapped destination fields
    $this->addUnmigratedDestinations(array(
      'path',
      'field_ding_staff_image:destination_dir',
      'field_ding_staff_image:destination_file',
      'field_ding_staff_image:file_replace',
      'field_ding_staff_image:source_dir',
      'field_ding_staff_image:urlencode',
    ));
  }

  public function prepareRow($row) {
    // Always include this snippet, in case our parent class decides to ignore the row
    if (parent::prepareRow($row) === FALSE) {
       return FALSE;
    }

    // Get values from serialized data and populate the source fields defined in our constructor
    // profile images
    $image_data = unserialize($row->field_image_data);
    $row->profile_image_alt = $image_data['alt'];
    $row->profile_image_title = $image_data['title'];

    // Populate the source fields defined in our constructor
    // Position to split the the staff_title_value is by the last space
    $split_pos = strrpos($row->nr_title, " ");
    if ($split_pos === false) {
      // Firstname blank
      $row->firstname_extracted = '';
      // Surname equals field_staff_title_value
      $row->surname_extracted = $row->nr_title;
    }
    else {
      // Firstname extracted from field_staff_title_value
      $row->firstname_extracted = substr($row->nr_title, 0, $split_pos);
      // Surname extracted from field_staff_title_value
      $row->surname_extracted = substr($row->nr_title, $split_pos);
    }

    // Populate departments
    $departments_array = array();
    // $dep_query = Database::getConnection('default', 'legacy')
    //     ->select('content_field_department', 'cfd')
    //     ->fields('cfd', array('field_department_value'))
    //     ->condition('nid', $row->nid)
    //     ->condition('vid', $row->vid);
    // $dep_result = $dep_query->execute();
    // foreach ($dep_result as $dep_row) {
    //   if ((!in_array($dep_row->field_department_value, $departments_array)) &&
    //       (!empty($dep_row->field_department_value))) {
    //     $departments_array[] = $dep_row->field_department_value;
    //   }
    // }
    $row->departments = $departments_array;

    //Populate libraries
    $libraries_array = array();
    $lib_query = Database::getConnection('default', 'legacy')
        ->select('content_field_multi_library_ref', 'ml')
        ->fields('ml', array('field_multi_library_ref_nid'))
        ->condition('nid', $row->nid)
        ->condition('vid', $row->vid);
    $lib_result = $lib_query->execute();
    foreach ($lib_result as $lib_row) {
      if ((!in_array($lib_row->field_multi_library_ref_nid, $libraries_array)) &&
          (!empty($lib_row->field_multi_library_ref_nid))) {
        $libraries_array[] = $lib_row->field_multi_library_ref_nid;
      }
    }
    $row->libraries = $libraries_array;
  }

  // Modify the query to include content profile data
  protected function query() {
    $query = Database::getConnection('default', 'legacy')
          ->select('node', 'n')
          ->fields('n', array('nid', 'title'))
          ->condition('n.type', 'profile', '=');
    $query->join('node_revisions', 'nr', 'n.vid = nr.vid');
    $query->addField('nr', 'vid');
    $query->addField('nr', 'body');
    $query->addField('nr', 'teaser');
    $query->addField('nr', 'title');
    $query->join('users', 'u', 'n.uid = u.uid');
    $query->addField('u', 'uid');
    $query->addField('u', 'name');
    $query->addField('u', 'mail');
    $query->join('content_field_image', 'cfi', 'n.nid = cfi.nid AND n.vid = cfi.vid');
    $query->addField('cfi', 'field_image_fid');
    $query->addField('cfi', 'field_image_data');
    $query->leftJoin('content_type_profile', 'ctp', 'n.nid = ctp.nid AND n.vid = ctp.vid');
    $query->addField('ctp','field_staff_title_value');
    $query->addField('ctp','field_work_area_value');
    // $query->addField('ctp','field_firstname_value');
    $query->addField('ctp', 'field_staff_phone_value');
    $query->join('ding_user', 'du', 'n.uid = du.uid');
    $query->addField('du', 'display_name');
    $query->orderBy('n.nid', 'ASC');
    // See more at: http://www.grasmash.com/article/migrate-classes-content-profile-profile2#sthash.5JUMSwgz.dpuf
    return $query;
  }
}
