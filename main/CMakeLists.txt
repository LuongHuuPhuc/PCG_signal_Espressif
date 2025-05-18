set(req driver freertos)
idf_component_register(SRCS "i2s_test.c"
                       INCLUDE_DIRS "." 
                       "D:/Esp-idf/Mysource/inmp441_test/Filter"
                       REQUIRES ${req}) 