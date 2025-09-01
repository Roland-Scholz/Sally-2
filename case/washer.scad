$fn = 15;

washer();


module washer() {
    difference(){
        cylinder(h=0.5, r=3.5);
            cylinder(h=0.5, r=1.75);
        
    };
}