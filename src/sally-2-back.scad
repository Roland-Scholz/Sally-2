$fn = 20;

thick = 2;
tf = 0.95;

xmax = 250;
ymax = 180+thick;
zmax = 39*2;

dev_x = 21.5;
dev_bot = 21;

edge=5;

//rotate([180,0,0])
    shape();

module shape() {
    difference(){
        base_shape();
        
        translate([dev_x, dev_bot, 0])
            sio();
        
        translate([dev_x+17.5+32, dev_bot, 0])
            sio();
        
        translate([dev_x+17.5+32+32+12, dev_bot, 0])
            serpar();

        translate([dev_x+17.5+32+32+12+86+3, dev_bot, 0])
            barrel();
        
        translate([dev_x+17.5+32+32+12+86+3+9+5, dev_bot, 0])
            usb();
        
    }
    
    /*
    translate([edge, zmax-4*thick-edge, -thick])
        cube([xmax-2*edge-4*thick, 3, 3]);

    translate([edge, 32, -thick])
        cube([xmax-2*edge-4*thick, 3, 3]);

    translate([edge, 5, -thick])
        cube([xmax-2*edge-4*thick, 3, 3]);
    */

}

module usb() {
    cube([12, 11, thick]);
}

module barrel() {
    cube([9, 11, thick]);
}

module serpar() {
    cube([86, 12.5, thick]);
}

module sio() {
    cube([32, 16, thick]);
}

module base_shape() {
    
    rotate([0,0,0])
    linear_extrude(thick * tf)
        base_shape_2d(thick * tf);

}

module base_shape_2d(th) {
    
    echo("width:", xmax-2*th);
    echo("height:", zmax-2*th);
    
    polygon([[0, zmax-edge-2*th], [0, edge], [edge,0], [xmax-edge-2*th, 0], [xmax-2*th, edge], [xmax-2*th, zmax-edge-2*th],[xmax-edge-2*th, zmax-2*th], [edge, zmax-2*th]  ]);
}
