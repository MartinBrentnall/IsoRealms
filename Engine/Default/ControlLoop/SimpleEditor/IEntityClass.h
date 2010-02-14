#ifndef I_ENTITY_CLASS_H
#define I_ENTITY_CLASS_H

#include <vector>
#include <string>

/**
 * Implement this interface if you wish to use the instantiation dialog to
 * instantiate something.
 */
class IEntityClass {
  public:  

  /**
   * Return the human readable name of this entity class.
   * 
   * @returns  The human readable name of this entity class.
   */
  virtual std::string getEntityClassName() = 0;

  /**
   * Create a new instance.
   * 
   * @param std::string&  The type to instantiate.
   * @param std::string&  The name of the new instance.
   */
  virtual void instantiate(std::string&, std::string&) = 0;

  /**
   * Remove an existing instance.
   * 
   * @param std::string&  The name of the instance to remove.
   */
  virtual void remove(std::string&) = 0;

  /**
   * Invoke a command that allows the specified instance to be configured.
   * 
   * @param std::string  The name of the instance to configure.
   */
  virtual void configure(std::string&) = 0;

  /**
   * Get a list of existing instances.
   * 
   * @returns  A vector containing names of existing instances.
   */
  virtual std::vector<std::string*> getInstances() = 0;

  /**
   * Get a list of implementations from which instances can be created.
   * 
   * @return  A vector containing names of available implementations.
   */
  virtual std::vector<std::string*> getImplementations() = 0;
};

#endif
