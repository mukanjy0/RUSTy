// rust.rs — casos para NameRes sin bloques arbitrarios

// 1. Función simple y llamada válida
fn foo(a: i32, b: i32) -> i32 {
    let x: i32 = a + b;
    return x;
}

fn main() {
    foo(1, 2);
    return ();
}

// 2. Llamada a función no declarada
fn test_undef_call()  {
    bar(1, 2);      // ERROR NameRes: 'bar' no está declarado
    return ();
}

// 3. Redeclaración en el mismo scope
fn test_redecl()  {
    let x: i32 = 1;
    let x: i32 = 2; // ERROR NameRes: redeclaration of 'x' in same scope
    return ();
}

// 4. Uso antes de declarar
fn test_use_before_decl()  {
    let y: i32 = x + 1;  // ERROR NameRes: 'x' no está declarado
    let x: i32 = 3;
    return ();
}

// 5. Parámetros disponibles en el scope de función
fn test_params(a: i32, b: i32)  {
    let s: i32 = a + b;  // OK: 'a' y 'b' existen como parámetros
    return ();
}

// 6. Asignación simple (NameRes solo enlaza nombre)
fn test_assign_mut()  {
    let mut m: i32 = 10;
    m = 20;              // NameRes: 'm' resuelto; TypeChecker revisará mutabilidad
    return ();
}

// 7. Asignación a inmutable (NameRes enlaza, TypeChecker falla luego)
fn test_assign_immut()  {
    let n: i32 = 5;
    n = 6;               // NameRes: 'n' resuelto; TypeChecker dará “immutable assignment”
    return ();
}

// 8. For introduce un scope para 'i'
fn test_for_loop()  {
    let arr: [i32; 3] = [1, 2, 3];
    for i in arr ..= 3 {
        let t: i32 = i + 1;  // OK
    }
    let z: i32 = i;          // ERROR NameRes: 'i' no está declarado en este scope
    return ();
}

// 9. Subíndice en array
fn test_subscript()  {
    let arr: [i32; 3] = [10, 20, 30];
    let v0: i32 = arr[0];    // OK
    let v1: i32 = arr[x];    // ERROR NameRes: 'x' no está declarado
    return ();
}

// 10. Printstmt (NameRes solo resuelve 'msg')
fn test_println()  {
    let msg: &str = "hello";
    println!("{msg}", 1, 2);     // NameRes: enlaza 'msg'; aridad/tipos van en TypeChecker
    return ();
}
