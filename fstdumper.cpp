#include "gpi.h"

#include <iostream>

void fstdumpter_entry()
{
    std::cout << "gpi_has_registered_impl: " << gpi_has_registered_impl() << std::endl;
    std::cout << "gpi_get_simulator_product: " << gpi_get_simulator_product() << std::endl;
    std::cout << "gpi_get_simulator_version: " << gpi_get_simulator_version() << std::endl;
}

int value_changed(void *gpi_cb_data)
{
    uint32_t low, high;
    gpi_get_sim_time(&high, &low);
    
    gpi_sim_hdl obj = (gpi_sim_hdl)gpi_cb_data;
    
    std::cout << "Value change: " << gpi_get_signal_name_str(obj) << "\tValue: " << gpi_get_signal_value_long(obj) << "\tTime: " << high << ", " << low << std::endl;
    
    //gpi_cb_hdl cb1 = gpi_register_value_change_callback(value_changed, obj, obj, GPI_RISING | GPI_FALLING);
    
    return 1;
}

void fstdumper_init(int argc, char const *const *argv)
{
    for (int i=0; i<argc; i++)
    {
        std::cout << argv[i] << std::endl;
    }

    gpi_sim_hdl root = gpi_get_root_handle("div_int_tb");

    int32_t precision;
    gpi_get_sim_precision(&precision);
    std::cout << "precision: " << precision << std::endl;
    
    if (!root)
    {
        std::cout << "Could not get root handle!" << std::endl;
        return;
    }
    
    std::cout << "root handle: " << gpi_get_signal_name_str(root) << std::endl;
    
    gpi_iterator_hdl iterator =  gpi_iterate(root, GPI_OBJECTS);
    
    gpi_sim_hdl obj;
    while ((obj = gpi_next(iterator)))
    {
        std::cout << "found object: " << gpi_get_signal_name_str(obj) << " of type: " << gpi_get_signal_type_str(obj) << std::endl;
        
        if (gpi_get_object_type(obj) == GPI_NET || gpi_get_object_type(obj) == GPI_REGISTER)
        {
            std::cout << "  -> register value change callback for net and reg" << std::endl;
            
            gpi_register_value_change_callback(value_changed, obj, obj, GPI_RISING | GPI_FALLING);
            //gpi_cb_hdl cb1 = gpi_register_value_change_callback(value_changed, obj, obj, GPI_RISING);
            //gpi_cb_hdl cb2 = gpi_register_value_change_callback(value_changed, obj, obj, GPI_FALLING);
        }
    }
}
