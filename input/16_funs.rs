fn fun() -> () {
    let a : i16 = 15;
    println!("The result is: {}", 1);
    fun2();
}

fn fun2() -> () {
    println!("The result is: {}", 2);
    fun3();
}

fn fun3() -> () {
    let x : i16 = 35 + 20;
    if x > 50 {
        for i in 0..=0 {
            println!("The result is: {}", 3);
        }
        for j in 0..0 {
            println!("The result is: {}", 3);
        }
    }
    else {
        println!("The result is: {}", 100);
    }
    fun4();
}

fn fun4() -> () {
    let x : i16 = 15;
    let y : i64 = 30;
    println!("The result is: {}", 4);
}

fn main(){
    fun();
}