<?php
/**
 * @file
 * Handling specific to a Ding1/D6 source for users
 */
class DingUserMigration extends DrupalUser6Migration {
  public function __construct(array $arguments) {
    $common_arguments = array(
      'description' => t('Migration of Users from Ding1/D6'),
      'machine_name' => 'DingUser',
      'group_name' => 'ding1_group',
      'source_connection' => 'legacy',
      'source_version' => 6,
      'role_migration' => 'DingRole',
    );

    parent::__construct(array_merge($arguments, $common_arguments));

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
      'og_user_node',
    ));

  }

  /**
   * Review a data row after fetch, returning FALSE to skip it.
   *
   * Function REPLACES prepareRow of parent (DrupalUser6Migration) - it doesn't append to it.
   * This is because the function does not begin with the lines:
   *
   *     if (parent::prepareRow($row) === FALSE) {
   *       return FALSE;
   *     }
   *
   * The purpose of this function replacement is to allow multiple users with same email adresses
   * to be created - the prepareRow() function in DrupalUser6Migration doesn't allow this.
   *
   * See DrupalUser6Migration's prepareRow() function to see original function.
   *
   * @param unknown $row
   * @return boolean
   */
  public function prepareRow($row) {
    /**
     * Copied over from migrate/includes/migration.inc
     */
    $this->rollbackAction = $this->defaultRollbackAction;


    /**
     * Modified from migrate_d2d/user.inc
     */
    // Map the admin account directly to the destination admin account.
    if ($row->uid == 1) {
      $new_uid = 1;
    }

    if (!empty($new_uid)) {
      $hash = isset($row->migrate_map_hash) ? $row->migrate_map_hash : NULL;
      $this->map->saveIDMapping($row, array($new_uid), MigrateMap::STATUS_IGNORED,
        MigrateMap::ROLLBACK_PRESERVE, $hash);
      $this->rollbackAction = MigrateMap::ROLLBACK_PRESERVE;
      return FALSE;
    }

    // Add the path to the source row, if relevant
    if ($this->moduleExists('path')) {
      $path = $this->version->getPath('user/' . $row->uid);
      if ($path) {
        $row->path = $path;
      }
    }

    // Pick up profile data.
    $this->version->getSourceValues($row, $row->uid);
    // Populate the source roles
    if ($this->roleMigration) {
      $result = Database::getConnection('default', $this->sourceConnection)
                ->select('users_roles', 'ur')
                ->fields('ur', array('rid'))
                ->condition('uid', $row->uid)
                ->execute();
      foreach ($result as $role_row) {
        $row->roles[] = $role_row->rid;
      }
    }


    /**
     * Copied over from migrate_d2d/d6/user.inc
     *
     * Note that the date module adds a timezone_name column to the users table,
     * so if present we can use that directly. Otherwise, we do as the D6->D7
     * upgrade does and just clear it - let users reset their timezones in the
     * D7 site.
     */
    if (!empty($row->timezone_name)) {
      $row->timezone = $row->timezone_name;
    }
    else {
      $row->timezone = NULL;
    }


    return TRUE;
  }

}
