CONTENTS OF THIS FILE
--------------------

Description
-----------
This module allows users to export all system specific blocks ( system specific
blocks are those blocks which are created using Drupal interface add block
functionality.) and then import it into another Drupal installation, or on the
same site.

Export
------
The blocks export interface allows the user to select a single or multiple
blocks to export.

User have choice to export each block with the Basic information
or Full Information

Import
------
The blocks import page accepts the custom array structure an an input which are
exported by using export.
feature of this module.

Before start of importing process administrator have a choice to override
existing block(s).

Installation
------------
1. Copy the entire block_export_import directory the Drupal sites/all/modules 
directory.

2. Login as an administrator. Enable the module in the "Administer" -> "Modules"

3. Access the links to to export the custom blocks 
(admin/structure/export-import-block)

3. Access the links to to Import the custom blocks 
(admin/structure/export-import-block/import)

DEPENDENCIES:
-------------
block

Support
-------
Please use the issue queue for filing bugs with this module at
https://drupal.org/node/2172541

AUTHOR:
-------
Devendra Yadav
dev.firoza@gmail.com
