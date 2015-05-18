<?php

/**
 * @file
 * Contains \GbVendorsMigrate.
 */

class GbVendorsMigrate extends GbMigration {

  public $entityType = 'node';
  public $bundle = 'vendor';

  public $csvColumns = array(
    array('body', 'Body'),
  );

  public function __construct() {
    parent::__construct();

    $this->addFieldMapping('body', 'body');

  }

}
