<?php

///////////////////////////////
// Terms

/**
 * Migrate Season status terms.
 */
class garmentboxSeasonStatusTerms extends garmentboxMigration {

  /**
   * Map the field and properties to the CSV header.
   */
  public $csvColumns = array(
    array('id', 'Unique ID'),
    array('name', 'Name'),
  );

  public $entityType = 'taxonomy_term';
  public $bundle = 'season_status';
}

/**
 * Migrate Item status terms.
 */
class garmentboxItemStatusTerms extends garmentboxMigration {

  public $csvColumns = array(
    array('id', 'Unique ID'),
    array('name', 'Name'),
  );

  public $entityType = 'taxonomy_term';
  public $bundle = 'item_status';
}

/**
 * Migrate Task status terms.
 */
class garmentboxTaskStatusTerms extends garmentboxMigration {

  public $csvColumns = array(
    array('id', 'Unique ID'),
    array('name', 'Name'),
    array('field_status_closed', 'Closed'),
  );

  public $entityType = 'taxonomy_term';
  public $bundle = 'task_status';

  public function __construct() {
    parent::__construct();
    $this->addFieldMapping('field_status_closed', 'field_status_closed');
  }
}

/**
 * Migrate Size terms.
 */
class garmentboxSizeTerms extends garmentboxMigration {

  public $csvColumns = array(
    array('id', 'Unique ID'),
    array('name', 'Name'),
    array('weight', 'Weight'),
  );

  public $entityType = 'taxonomy_term';
  public $bundle = 'size';

  public function __construct() {
    parent::__construct();
    $this->addFieldMapping('weight', 'weight');
  }
}

/**
 * Migrate Size terms.
 */
class garmentboxInventoryTypeTerms extends garmentboxMigration {

  public $csvColumns = array(
    array('id', 'Unique ID'),
    array('name', 'Name'),
    array('field_include_in_total', 'Include in total'),
    array('field_in_warehouse', 'In warehouse'),
    array('field_closed_stock', 'Closed stock'),
  );

  public $entityType = 'taxonomy_term';
  public $bundle = 'inventory_type';

  public function __construct() {
    parent::__construct();
    $this->addFieldMapping('field_include_in_total', 'field_include_in_total');
    $this->addFieldMapping('field_in_warehouse', 'field_in_warehouse');
    $this->addFieldMapping('field_closed_stock', 'field_closed_stock');
  }
}

/**
 * Migrate measurement unit terms.
 */
class garmentboxMeasurementUnitTerms extends garmentboxMigration {

  public $csvColumns = array(
    array('id', 'Unique ID'),
    array('name', 'Name'),
    array('field_unit_type', 'Type'),
    array('field_conversion_ratio', 'Conversion'),
  );

  public $entityType = 'taxonomy_term';
  public $bundle = 'measurement_units';

  public function __construct() {
    parent::__construct();
    $this->addFieldMapping('field_unit_type', 'field_unit_type');
    $this->addFieldMapping('field_conversion_ratio', 'field_conversion_ratio');
  }
}

/**
 * Migrate BOL terms.
 */
class garmentboxBolTerms extends garmentboxMigration {

  public $csvColumns = array(
    array('id', 'Unique ID'),
    array('name', 'Name'),
  );

  public $entityType = 'taxonomy_term';
  public $bundle = 'bol';
}

/**
 * Migrate order status terms.
 */
class garmentboxOrderStatusTerms extends garmentboxMigration {

  public $csvColumns = array(
    array('id', 'Unique ID'),
    array('name', 'Name'),
  );

  public $entityType = 'taxonomy_term';
  public $bundle = 'order_status';
}

///////////////////////////////
// Nodes

/**
 * Migrate Department nodes.
 */
class garmentboxDepartments extends garmentboxMigration {

  public $csvColumns = array(
    array('id', 'Unique ID'),
    array('title', 'Title'),
  );

  public $entityType = 'node';
  public $bundle = 'department';

  public function complete($node) {
    if ($node->title == 'Main') {
      variable_set('garmentbox_og_main_department', $node->nid);
    }
  }
}

/**
 * Migrate Collection nodes.
 */
class garmentboxSeasons extends garmentboxMigration {

  public $csvColumns = array(
    array('id', 'Unique ID'),
    array('title', 'Title'),
    array('body', 'Body'),
    array('field_season_status', 'Status'),
  );

  public $entityType = 'node';
  public $bundle = 'season';

  public $dependencies = array(
    'garmentboxDepartments',
    'garmentboxSeasonStatusTerms',
  );

  public function __construct() {
    parent::__construct();
    $this->addFieldMapping('body', 'body');
    $this
      ->addFieldMapping('field_season_status', 'field_season_status')
      ->sourceMigration('garmentboxSeasonStatusTerms');
  }
}

/**
 * Migrate Address nodes.
 */
class garmentboxAddresses extends garmentboxMigration {

  public $csvColumns = array(
    array('id', 'Unique ID'),
    array('title', 'Title'),
    array('profile_address', 'Address'),
    array('profile_city', 'City'),
    array('profile_state', 'State'),
    array('profile_country', 'Country'),
  );

  public $entityType = 'node';
  public $bundle = 'address';

  public $dependencies = array(
    'garmentboxDepartments',
  );

  public function __construct() {
    parent::__construct();

    $arguments = array(
      'thoroughfare' => array('source_field' => 'profile_address'),
      'locality' => array('source_field' => 'profile_city'),
      'administrative_area' => array('source_field' => 'profile_state'),
    );
    // The country should be passed in as the primary value.
    $this->addFieldMapping('field_postal_address', 'profile_country')
         ->arguments($arguments)
         ->description('Address field');
    // Since the excerpt is mapped via an argument, add a null mapping so it's
    // not flagged as unmapped.
    $this->addFieldMapping(NULL, 'profile_address');
    $this->addFieldMapping(NULL, 'profile_city');
    $this->addFieldMapping(NULL, 'profile_state');
  }
}

/**
 * Migrate Vendors nodes.
 */
class garmentboxVendors extends garmentboxMigration {

  public $csvColumns = array(
    array('id', 'Unique ID'),
    array('title', 'Title'),
    array('field_address_inline', 'Address'),
  );

  public $entityType = 'node';
  public $bundle = 'vendor';

  public $dependencies = array(
    'garmentboxDepartments',
    'garmentboxAddresses',
  );

  public function __construct() {
    parent::__construct();
    $this->addFieldMapping('body', 'body');

    $this
      ->addFieldMapping('field_address_inline', 'field_address_inline')
      ->sourceMigration('garmentboxAddresses');
  }
}

/**
 * Migrate Customers nodes.
 */
class garmentboxCustomers extends garmentboxMigration {

  public $csvColumns = array(
    array('id', 'Unique ID'),
    array('title', 'Title'),
    array('field_addresses_inline', 'Address'),
  );

  public $entityType = 'node';
  public $bundle = 'customer';

  public $dependencies = array(
     'garmentboxAddresses',
  );

  public function __construct() {
    parent::__construct();

    $this
      ->addFieldMapping('field_addresses_inline', 'field_addresses_inline')
      ->sourceMigration('garmentboxAddresses');
  }

  function prepareRow($row) {
    parent::prepareRow($row);
    $row->field_addresses_inline = explode('; ', $row->field_addresses_inline);
  }
}

/**
 * Migrate Materials nodes.
 */
class garmentboxMaterials extends garmentboxMigration {

  public $csvColumns = array(
    array('id', 'Unique ID'),
    array('title', 'Title'),
    array('body', 'Body'),
    array('field_nick_name', 'Nick name'),
    array('field_images', 'Images'),
  );

  public $entityType = 'node';
  public $bundle = 'material';

  public $dependencies = array(
    'garmentboxDepartments',
    'garmentboxVendors',
  );

  public function __construct() {
    parent::__construct();

    $this->addFieldMapping('body', 'body');
    $this->addFieldMapping('field_nick_name', 'field_nick_name');

    $this->addFieldMapping('field_images', 'field_images');
    $this
      ->addFieldMapping('field_images:file_replace')
      ->defaultValue(FILE_EXISTS_REPLACE);
    $this
      ->addFieldMapping('field_images:source_dir')
      ->defaultValue(drupal_get_path('module', 'garmentbox_migrate') . '/images');
  }
}

/**
 * Migrate Items nodes.
 */
class garmentboxItems extends garmentboxMigration {

  public $csvColumns = array(
    array('id', 'Unique ID'),
    array('title', 'Title'),
    array('body', 'Body'),
    array('field_season', 'Season'),
    array('field_item_status', 'Status'),
    array('field_image', 'Image'),
  );

  public $entityType = 'node';
  public $bundle = 'item';

  public $dependencies = array(
    'garmentboxDepartments',
    'garmentboxItemStatusTerms',
    'garmentboxSeasons',
  );

  public function __construct() {
    parent::__construct();
    $this->addFieldMapping('body', 'body');
    $this
      ->addFieldMapping('field_season', 'field_season')
      ->sourceMigration('garmentboxSeasons');

    $this
      ->addFieldMapping('field_item_status', 'field_item_status')
      ->sourceMigration('garmentboxItemStatusTerms');

    $this->addFieldMapping('field_image', 'field_image');
    $this
      ->addFieldMapping('field_image:file_replace')
      ->defaultValue(FILE_EXISTS_REPLACE);
    $this
      ->addFieldMapping('field_image:source_dir')
      ->defaultValue(drupal_get_path('module', 'garmentbox_migrate') . '/images');
  }
}

/**
 * Migrate Task nodes.
 */
class garmentboxTasks extends garmentboxMigration {

  public $csvColumns = array(
    array('id', 'Unique ID'),
    array('title', 'Title'),
    array('body', 'Body'),
    array('field_entity_reference', 'Reference'),
    array('field_task_status', 'Status'),
  );

  public $entityType = 'node';
  public $bundle = 'task';

  public $dependencies = array(
    'garmentboxDepartments',
    'garmentboxTaskStatusTerms',
    'garmentboxSeasons',
    'garmentboxItems',
    'garmentboxItemVariants',
  );

  public function __construct() {
    parent::__construct();
    $this->addFieldMapping('body', 'body');
    $this
      ->addFieldMapping('field_entity_reference', 'field_entity_reference')
      ->sourceMigration(array('garmentboxSeasons', 'garmentboxItems','garmentboxItemVariants'));

    $this
      ->addFieldMapping('field_task_status', 'field_task_status')
      ->sourceMigration('garmentboxTaskStatusTerms');
  }
}

/**
 * Migrate Item variants nodes.
 */
class garmentboxItemVariants extends garmentboxMigration {

  public $csvColumns = array(
    array('id', 'Unique ID'),
    array('title', 'Title'),
    array('field_item', 'Item'),
    array('field_images', 'Images'),
    array('field_item_status', 'Status'),
    array('field_retail_price', 'Retail price'),
    array('field_wholesale_price', 'Wholesale price'),
  );

  public $entityType = 'node';
  public $bundle = 'item_variant';

  public $dependencies = array(
    'garmentboxDepartments',
    'garmentboxItems',
    'garmentboxItemStatusTerms',
  );

  public function __construct() {
    parent::__construct();
    $this->addFieldMapping('body', 'body');
    $this
      ->addFieldMapping('field_item', 'field_item')
      ->sourceMigration('garmentboxItems');


    $this->addFieldMapping('field_images', 'field_images');
    $this
      ->addFieldMapping('field_images:file_replace')
      ->defaultValue(FILE_EXISTS_REPLACE);
    $this
      ->addFieldMapping('field_images:source_dir')
      ->defaultValue(drupal_get_path('module', 'garmentbox_migrate') . '/images');

    $this
      ->addFieldMapping('field_item_status', 'field_item_status')
      ->sourceMigration('garmentboxItemStatusTerms');

    $this->addFieldMapping('field_retail_price', 'field_retail_price');

    $this->addFieldMapping('field_wholesale_price', 'field_wholesale_price');
  }

  function prepareRow($row) {
    parent::prepareRow($row);
    $row->field_images = explode('; ', $row->field_images);
  }
}

/**
 * Migrate inventory line nodes.
 */
class garmentboxInventoryLines extends garmentboxMigration {

  public $csvColumns = array(
    array('id', 'Unique ID'),
    array('title', 'Title'),
    array('field_item_variant', 'Item variant'),
    array('field_inventory_type', 'Type'),
    array('field_production_start_date', 'Production date'),
    array('field_delivery_date', 'Delivery date'),
    array('field_inventory_status', 'Status'),
  );

  public $entityType = 'node';
  public $bundle = 'inventory_line';

  public $dependencies = array(
    'garmentboxDepartments',
    'garmentboxItemVariants',
    'garmentboxInventoryTypeTerms',
  );

  protected $dateFields = array(
    'field_production_start_date',
    'field_delivery_date'
  );

  public function __construct() {
    parent::__construct();

    $this
      ->addFieldMapping('field_item_variant', 'field_item_variant')
      ->sourceMigration('garmentboxItemVariants');

    $this
      ->addFieldMapping('field_inventory_type', 'field_inventory_type')
      ->sourceMigration('garmentboxInventoryTypeTerms');

    $this->addFieldMapping('field_production_start_date', 'field_production_start_date');
    $this->addFieldMapping('field_delivery_date', 'field_delivery_date');

    $this->addFieldMapping('field_inventory_status', 'field_inventory_status');
  }
}

/**
 * Migrate order nodes.
 */
class garmentboxOrders extends garmentboxMigration {

  public $csvColumns = array(
    array('id', 'Unique ID'),
    array('title', 'Title'),
    array('field_season', 'Season'),
    array('field_customer', 'Customer'),
    array('field_address', 'Address'),
    array('field_inventory_line_inline', 'Inventory line'),
    array('field_order_status', 'Status'),
  );

  public $entityType = 'node';
  public $bundle = 'order';

  public $dependencies = array(
    'garmentboxSeasons',
    'garmentboxCustomers',
    'garmentboxAddresses',
    'garmentboxInventoryLines',
    'garmentboxOrderStatusTerms'
  );

  public function __construct() {
    parent::__construct();

    $this
      ->addFieldMapping('field_season', 'field_season')
      ->sourceMigration('garmentboxSeasons');

    $this
      ->addFieldMapping('field_customer', 'field_customer')
      ->sourceMigration('garmentboxCustomers');

    $this
      ->addFieldMapping('field_address', 'field_address')
      ->sourceMigration('garmentboxAddresses');

    $this
      ->addFieldMapping('field_inventory_line_inline', 'field_inventory_line_inline')
      ->sourceMigration('garmentboxInventoryLines')
      ->separator('|');

    $this
      ->addFieldMapping('field_order_status', 'field_order_status')
      ->sourceMigration('garmentboxOrderStatusTerms');
  }

  public function prepare($entity, $row) {
    // Fix the field_inventory_line_inline values.
    foreach ($entity->field_inventory_line_inline[LANGUAGE_NONE] as $delta => $values) {
      $target_id = $values['target_id']['destid1'];
      $entity->field_inventory_line_inline[LANGUAGE_NONE][$delta]['target_id'] = $target_id;
    }
  }
}

/**
 * Migrate bubble nodes.
 */
class garmentBoxBubbles extends garmentboxMigration {

  public $csvColumns = array(
    array('id', 'Unique ID'),
    array('field_bubble_id', 'Bubble ID'),
    array('title', 'Title'),
    array('body', 'Text'),
    array('field_css_selector', 'CSS selector'),
    array('field_display_mode', 'Display mode'),
  );

  public $entityType = 'node';
  public $bundle = 'bubble';

  public function __construct() {
    parent::__construct();

    $this->addFieldMapping('field_bubble_id', 'field_bubble_id');
    $this->addFieldMapping('body', 'body');
    $this->addFieldMapping('field_css_selector', 'field_css_selector');
    $this->addFieldMapping('field_display_mode', 'field_display_mode');
  }
}

/**
 * Migrate factory nodes.
 */
class garmentBoxFactories extends garmentboxMigration {

  public $csvColumns = array(
    array('id', 'Unique ID'),
    array('title', 'Title'),
  );

  public $entityType = 'node';
  public $bundle = 'factory';
}

///////////////////////////////
// Field collections

/**
 * Migrate "Status info" field collection.
 */
class garmentboxCollectionsFieldStatusInfo extends garmentboxFieldCollectionMigration {
  public $csvColumns = array(
    array('id', 'Unique ID'),
    array('host_entity_id', 'Season'),
    array('field_season_status', 'Status'),
    array('field_date', 'Deadline'),
  );

  public $entityType = 'field_collection_item';
  public $bundle = 'field_status_info';
  public $hostEntityType = 'node';

  public $dependencies = array(
    'garmentboxSeasonStatusTerms',
    'garmentboxSeasons',
  );

  protected $dateFields = array(
    'field_date',
  );

  public function __construct() {
    parent::__construct();
    $this
      ->addFieldMapping('host_entity_id', 'host_entity_id')
      ->sourceMigration('garmentboxSeasons');

    $this
      ->addFieldMapping('field_season_status', 'field_season_status')
      ->sourceMigration('garmentboxSeasonStatusTerms');

    $this->addFieldMapping('field_date', 'field_date');
  }
}


/**
 * Migrate "Size info" field collection.
 */
class garmentboxCollectionsFieldSizeInfo extends garmentboxFieldCollectionMigration {
  public $csvColumns = array(
    array('id', 'Unique ID'),
    array('host_entity_id', 'Item variant'),
    array('field_size', 'Size'),
    array('field_sku', 'SKU'),
  );

  public $entityType = 'field_collection_item';
  public $bundle = 'field_size_info';
  public $hostEntityType = 'node';

  public $dependencies = array(
    'garmentboxSizeTerms',
    'garmentboxItemVariants',
  );

  public function __construct() {
    parent::__construct();
    $this
      ->addFieldMapping('host_entity_id', 'host_entity_id')
      ->sourceMigration('garmentboxItemVariants');

    $this
      ->addFieldMapping('field_size', 'field_size')
      ->sourceMigration('garmentboxSizeTerms');

    $this->addFieldMapping('field_sku', 'field_sku');
  }
}


/**
 * Migrate "Source info" field collection.
 */
class garmentboxCollectionsFieldSourceInfo extends garmentboxFieldCollectionMigration {
  public $csvColumns = array(
    array('id', 'Unique ID'),
    array('host_entity_id', 'Material'),
    array('field_vendor', 'Vendor'),
    array('field_price', 'Price'),
    array('field_unit', 'Unit'),
    array('field_length', 'Length'),
    array('field_length_unit', 'Length unit'),
  );

  public $entityType = 'field_collection_item';
  public $bundle = 'field_source_info';
  public $hostEntityType = 'node';

  public $dependencies = array(
    'garmentboxMaterials',
    'garmentboxVendors',
    'garmentboxMeasurementUnitTerms',
  );

  public function __construct() {
    parent::__construct();
    $this
      ->addFieldMapping('host_entity_id', 'host_entity_id')
      ->sourceMigration('garmentboxMaterials');

    $this
      ->addFieldMapping('field_vendor', 'field_vendor')
      ->sourceMigration('garmentboxVendors');

    $this->addFieldMapping('field_price', 'field_price');

    $this
      ->addFieldMapping('field_unit', 'field_unit')
      ->sourceMigration('garmentboxMeasurementUnitTerms');

    $this->addFieldMapping('field_length', 'field_length');

    $this
      ->addFieldMapping('field_length_unit', 'field_length_unit')
      ->sourceMigration('garmentboxMeasurementUnitTerms');
  }
}

/**
 * Migrate "Bom info" field collection.
 */
class garmentboxCollectionsFieldBomInfo extends garmentboxFieldCollectionMigration {
  public $csvColumns = array(
    array('id', 'Unique ID'),
    array('host_entity_id', 'Item variant'),
    array('field_material_item', 'Material source'),
    array('field_unit', 'Unit'),
    array('field_quantity_decimal', 'Quantity'),
  );

  public $entityType = 'field_collection_item';
  public $bundle = 'field_bom_info';
  public $hostEntityType = 'node';

  public $dependencies = array(
    'garmentboxItemVariants',
    'garmentboxMaterials',
    'garmentboxCollectionsFieldSourceInfo',
    'garmentboxMeasurementUnitTerms',
  );

  public function __construct() {
    parent::__construct();
    $this
      ->addFieldMapping('host_entity_id', 'host_entity_id')
      ->sourceMigration('garmentboxItemVariants');

    $this
      ->addFieldMapping('field_material_item', 'field_material_item')
      ->sourceMigration('garmentboxCollectionsFieldSourceInfo');

    $this
      ->addFieldMapping('field_unit', 'field_unit')
      ->sourceMigration('garmentboxMeasurementUnitTerms');

    $this->addFieldMapping('field_quantity_decimal', 'field_quantity_decimal');
  }
}

/**
 * Migrate "Bol info" field collection.
 */
class garmentboxCollectionsFieldBolInfo extends garmentboxFieldCollectionMigration {
  public $csvColumns = array(
    array('id', 'Unique ID'),
    array('host_entity_id', 'Item variant'),
    array('field_bol_term', 'Labour'),
    array('field_price', 'Price'),
  );

  public $entityType = 'field_collection_item';
  public $bundle = 'field_bol_info';
  public $hostEntityType = 'node';

  public $dependencies = array(
    'garmentboxItemVariants',
    'garmentboxBolTerms'
  );

  public function __construct() {
    parent::__construct();
    $this
      ->addFieldMapping('host_entity_id', 'host_entity_id')
      ->sourceMigration('garmentboxItemVariants');

    $this
      ->addFieldMapping('field_bol_term', 'field_bol_term')
      ->sourceMigration('garmentboxBolTerms');

    $this->addFieldMapping('field_price', 'field_price');
  }
}

/**
 * Migrate "Quantity info" field collection.
 */
class garmentboxCollectionsFieldQuantityInfo extends garmentboxFieldCollectionMigration {
  public $csvColumns = array(
    array('id', 'Unique ID'),
    array('host_entity_id', 'Inventory line'),
    array('field_size', 'Size'),
    array('field_quantity_integer', 'Quantity'),
  );

  public $entityType = 'field_collection_item';
  public $bundle = 'field_quantity_info';
  public $hostEntityType = 'node';

  public $dependencies = array(
    'garmentboxInventoryLines',
    'garmentboxSizeTerms',
  );

  public function __construct() {
    parent::__construct();
    $this
      ->addFieldMapping('host_entity_id', 'host_entity_id')
      ->sourceMigration('garmentboxInventoryLines');

    $this
      ->addFieldMapping('field_size', 'field_size')
      ->sourceMigration('garmentboxSizeTerms');

    $this->addFieldMapping('field_quantity_integer', 'field_quantity_integer');
  }
}