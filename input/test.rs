// fn fun(num : i32, num2 : ())

fn fun(num : i32)
{
    // -> i32 {
    let num : char = 'a';
    5
    // 1 + 5;
    // return 6
}

// static MYGLOBAL : i32 = 80;
// const MYGLOBAL2 : i32 AAAAA

fn main() {
    // Statements here are executed when the compiled binary is called.
    let mut i : i32 = fun(8);
    let nv : i32 = fun(84);
    // const g : i32 = 1 * 8;
    println!("{}", i);

    let s; // does not have size
    s = "hello"; // does not have size
    // | h | e | l | l | o |
    let crazy : [&str ; 2] = ["aa", "abc"];
    // let ss : &str = &s[..(('a' as usize) - 93)]; // &s[0..1] &s[1..] &s[..3] &s[..]
    let ss : &str = &s[..4]; // &s[0..1] &s[1..] &s[..3] &s[..]
    let s2 : &str;
    s2 = &ss[..]; // AHHHHHH
    // | h | e | l | l |
    println!("{s2}");
    let mut xs: [i32; 5];
    xs = [10; 5];
    let mut xp : &i32 = &xs[3];
    // let mut xp : &i32 = &mut xs[3];
    xp = &i;
    // println!("this should be 4: {}", xp);
    // &xp = xs[4]
    xs[3] = 7;
    // xs[3] = 5;
    println!("this should be 5: {}", xs[3]);
    let mut cs: [char; 2] = ['a' ; 2];
    let mut is: [i32; 2] = [i + 8 ; 2];
    let mut ys;
    let empty = ();
    // let empty : () = ();
    ys = [0, 0, 0, 0];
    println!("{}", ys[3]);
    // println!("{}", xs[(xs[0] + 1)]);
    // println!("{}", xs[(xs[0] + 1) as usize]);
    xs[0] = 2;
    println!("{}", xs[0]);
    let c = loop {
        break 0;
        s = "hi";
    };

    println!("{}", c);

    i < 6;

    // if if i > 6 { 0 } else { 1 } > 0 { println!("hello\n"); } else { println!("goodbye\n"); }
    if i < 6 { println!("hello\n"); } else { println!("goodbye\n"); }
    let fifi = 4;

    for j in i..=5 {
        println!("{}", j);
        // i + = j;
    }

    if i > 4 {
        return
    }
    // return if i > 5 { let jaja = 8; } else { let jojo = 9; }
    // return TODO
    println!("{}", (if i > 6 {"Hello World!"} else {"Goodbye world!"}));

    // Print text to the console.
    // let mut ch = '\n';
    return
}