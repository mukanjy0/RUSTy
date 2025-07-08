fn main(){
    let strt: &str = "Hello, World!";
    let mut s2 : &str = "Heyyyy";
    println!("{}", s2);
    s2 = "hiiii";
    println!("{}", s2);
    s2 = strt;
    println!("{}", s2);
}
