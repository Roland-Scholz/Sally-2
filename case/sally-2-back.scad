$fn = 20;

thick = 2;
tf = 1.2;

xmax = 250;
ymax = 180+thick;
zmax = 39*2;

dev_x = 21.5;
dev_bot = 21;

edge=5;

rotate([180,0,0])
    shape();

module shape() {
    difference(){
        base_shape();
        
        translate([dev_x, dev_bot, 0])
            sio();
        
        translate([dev_x+17.5+32, dev_bot, 0])
            sio();
        
        translate([dev_x+17.5+31+32+13, dev_bot+1, 0])
            par();
        
        translate([dev_x+17.5+31+32+13+53+2, dev_bot+1, 0])
            ser();

        translate([dev_x+17.5+32+32+12+86+2, dev_bot, 0])
            barrel();
/*        
        translate([dev_x+17.5+32+32+12+86+3+9+5, dev_bot, 0])
            usb();
*/
        translate([60+60.5, 63,0])
            floppy();
        
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
    cube([9.5, 11.5, thick]);
}

module par() {
    cube([53, 11.5, thick]);
}

module ser() {
    cube([30.5, 11.5, thick]);
}

module sio() {
    cube([32, 16, thick]);
}

module base_shape() {
    
    rotate([0,0,0])
    linear_extrude(1.6)
        base_shape_2d(thick * tf);
    
    translate([60+60.5+8.5, 58, -7])
        spacer_round();
    translate([49, 58, -7])
        spacer_round();

}

module base_shape_2d(th) {
    
    echo("width:", xmax-2*th);
    echo("height:", zmax-2*th);
    
    polygon([[0, zmax-edge-2*th], [0, edge], [edge,0], [xmax-edge-2*th, 0], [xmax-2*th, edge], [xmax-2*th, zmax-edge-2*th],[xmax-edge-2*th, zmax-2*th], [edge, zmax-2*th]  ]);
}

module floppy() {
    rotate([0,0,180])
    translate([0,0,-thick]) {
    cube([60, 11, 2*thick]);
    
    translate([60+11.5, 5, 0])
        cylinder(2*thick, 2, 2);
    
    translate([-8.5, 5, 0])
        cylinder(2*thick, 2, 2);
    }
}

module spacer_round() {
    difference(){
        cylinder(h=7, r=3);
            cylinder(h=7, r=1.6);
        
    };
}