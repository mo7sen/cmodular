# Pseudo Design doc of cmodular

The module system should be able to handle the functionality 
of the following:
  - modulesystem
  - module
  - moduleinterface
  - modulecategory


The modulesystem should be able to:
  - [x] Add modules
  - [x] Find modules by name
  - [x] Find modules by category


Modulesystem |-> Modules |-> Name
             |           |
             |           |-> Categories |-> Name
             |                          |
             |                          |-> Interface
             |
             |-> Cached Categories

Notes:
- [x] A modulesystem can have an arbitrary number of modules
- Cached categories are categories that the modulesystem was asked to
  get before
- [-] A module's name is a unique identifier that shouldn't be repeated in
  a modulesystem
- [x] Each module can have an arbitrary number of categories
- [x] A category contains a Name (unique) and an (optional) interface
- [x] A category's name can be used to find it.
- [x] An interface is a list of functions that the module implements and
  exposes via the category.

- ~~Each interface type should be bound to a category name. This is so
  that an interface can only be added to specific categories
  **Example**: 
  * AdditionInterface tied to "addition" category.
  * An instance of AdditionInterface can be added to category "addition"
  * Adding AdditionInterface to category "multiplication" results in an error~~
  The above relation was fulfilled in the opposite direction. *Each category is
  bound to a specific interface and can only implement its functions.*

Behaviour:

**ModuleSystem**
  - addmodule:
    1. Check if a module exists with the same name, return err if true
    2. Store the module's categories for more direct accessing later
      2.1. If a category already exists, don't update it ( the module
           accepts the first interface as being the primary one and
           ignores all subsequent ones.
  - removemodule:
    1. If it exists, remove and return
      1.1. If the module had a primary interface stored in it,
           it should be removed from the modulesystem's cache.
    2. If it doesn't exist, return NULL
  - getmodule:
    1. Searches for module by name
    2. NULL if not found
  - init:
    1. Initializes the modulesystem
  - deinit:
    1. Deinitializes the modulesystem
    2. Deinitializes all modules in it

**Module**
  - init:
    1. Create a module
    2. Name to be used as a unique identifier should be provided
  - deinit:
    1. Destroys the module
    2. Doing this to a module that is registered in a modulesystem
       is an error that **should** be detected
       2.1. Should the module have a reference to the modulesystem?
  - addcategory:


