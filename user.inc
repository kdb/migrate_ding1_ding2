<?php
/**
 * @file
 * Handling specific to a Ding1/D6 source for users
 */
class DingUserMigration extends DrupalUser6Migration {
  public function __construct(array $arguments) {
    $arguments = array(
      'description' => t('Migration of Users from Ding1/D6'),
      'machine_name' => 'DingUser',
      'group_name' => 'ding1_group',
      'source_connection' => 'legacy',
      'source_version' => 6,
      'role_migration' => 'DingRole',
    );

    parent::__construct($arguments);

    // Unmapped source fields
    $this->addUnmigratedSources(array(
      'field_department',
      'field_image',
      'field_image:list',
      'field_image:alt',
      'field_multi_library_ref',
      'field_staff_title',
      'field_work_area',
      'field_firstname',
      'field_phone',
      'field_surname',
      '2',
      'field_profile_title',
      'field_profile_body',
    ));

    // Unmapped destination fields
    $this->addUnmigratedDestinations(array(
      'group_audience',
      'group_audience:state',
      'group_audience:created',
    ));

  }

   public function prepareRow($row) {
    // If user has a mail address, run check for existing mail address as same email for 2 users stops user_save process
    if (!empty($row->mail)) {
      // Check if user with the same email already exists in destination table "users"
      if (user_load_by_mail($row->mail)) {
        //user found - modify email before saving (to ensure user is saved as a new distinct user)
        $email_found = $row->mail;
        $email_parts = explode("@", $email_found);
        $email_modified = $email_parts[0] . "_1@" . $email_parts[1];
        $row->mail = $email_modified;

        // Logging
        watchdog('migrate_ding1_ding2', 'User with same email (%email_existing) found in destination - modifying email (to %email_new) to force a new distinct user to be created', array('%email_existing'=>$email_found, '%email_new' => $email_modified), WATCHDOG_INFO);
      }
    }
  }

}
