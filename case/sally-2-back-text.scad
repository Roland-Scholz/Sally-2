beschriftung();

module beschriftung() {
    
    translate([60, -47, -1.6])
    linear_extrude(0.2){
        rotate([180, 0, 0])
            text("1                SHUGART", 3, "Arial:style=Bold");
    }

     translate([33, -16, -1.6])
    linear_extrude(0.2){
        rotate([180, 0, 0])
            text("SIO                                      SIO                                 CENTRONICS                       RS232          +5V", 3, "Arial:style=Bold");
    }

}