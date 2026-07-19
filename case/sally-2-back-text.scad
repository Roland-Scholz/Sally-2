beschriftung();

module beschriftung() {
    
    translate([60, -46, 0])
    linear_extrude(0.4){
        rotate([180, 0, 0])
            text("1                SHUGART", 3, "Arial:style=Bold");
    }

    translate([33, -16, 0])
    linear_extrude(0.4){
        rotate([180, 0, 0])
            text("SIO                                      SIO                                 CENTRONICS                       RS232          Vin    FUNC", 3, "Arial:style=Bold");
    }

}