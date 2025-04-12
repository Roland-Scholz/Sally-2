$fn = 20;

thick = 2;
tf = 1.2;

xmax = 250;
ymax = 180+thick;
zmax = 39*2;

ledx = 141;
ledy = 12;

edge=5;

//rotate([180,0,0])
    shape();

module shape() {
    
    echo("Floppy start:", 14);
    echo("Switch start:", 32);
    echo("Switch start:", 75);
    echo("Switch start:", 94);
    
    difference(){
        base_shape();
        
        translate([32, 10, 0])
            switch();
        translate([76, 10, 0])
            switch();
        translate([93.5, 10, 0])
            switch();
        
        translate([216, 26, 0])
            reset();
        
        translate([14, 35, 0])
            floppy();
        translate([128, 35, 0])
            floppy();

        translate([ledx, ledy, 0])
            led();
        translate([ledx+15, ledy, 0])
            led();
        translate([ledx+2*15, ledy, 0])
            led();
            
        translate([ledx, ledy+15, 0])
            led();
        translate([ledx+15, ledy+15, 0])
            led();
        translate([ledx+2*15, ledy+15, 0])
            led();
        translate([ledx+3*15, ledy+15, 0])
            led();
        
    }
    
    translate([edge, zmax-4*thick-edge, -thick])
        cube([xmax-2*edge-4*thick, 3, 3]);

    translate([edge, 32, -thick])
        cube([xmax-2*edge-4*thick, 3, 3]);

    translate([edge, 5, -thick])
        cube([xmax-2*edge-4*thick, 3, 3]);

}

module led() {
    linear_extrude(thick*tf)
        circle(1.6); 
}

module floppy() {
    linear_extrude(thick*tf)
        square([102 + 0.1, 26 + 0.1]);
}

module reset() {
    linear_extrude(thick*tf)
        circle(5.5); 
}

module switch() {
    linear_extrude(thick*tf)
        square([15 + 0.3, 21 + 0.3]);
}

module base_shape() {
    
    rotate([0,0,0])
    linear_extrude(thick * 0.95)
        base_shape_2d(thick * 0.95);

}

module base_shape_2d(th) {
    
    echo("width:", xmax-2*th);
    echo("height:", zmax-2*th);
    
    polygon([[0, zmax-edge-2*th], [0, edge], [edge,0], [xmax-edge-2*th, 0], [xmax-2*th, edge], [xmax-2*th, zmax-edge-2*th],[xmax-edge-2*th, zmax-2*th], [edge, zmax-2*th]  ]);
}
