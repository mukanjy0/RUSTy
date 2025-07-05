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
    //bar(1, 2);      // ERROR NameRes: 'bar' no está declarado
    return ();
}

// 3. Redeclaración en el mismo scope
fn test_redecl()  {
    let x: i32 = 1;
    //let x: i32 = 2; // ERROR NameRes: redeclaration of 'x' in same scope
    return ();
}

// 4. Uso antes de declarar
fn test_use_before_decl()  {
    //let y: i32 = x + 1;  // ERROR NameRes: 'x' no está declarado
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
    //n = 6;               // NameRes: 'n' resuelto; TypeChecker dará “immutable assignment”
    return ();
}

// 8. For introduce un scope para 'i'
fn test_for_loop()  {
    //let arr: [i32; 3] = [1, 2, 3];
    //for i in arr ..= 3 {
        //let t: i32 = i + 1;  // OK
    //}
    //let z: i32 = i;          // ERROR NameRes: 'i' no está declarado en este scope
    return ();
}

// 9. Subíndice en array
fn test_subscript()  {
    //let arr: [i32; 3] = [10, 20, 30];
    //let v0: i32 = arr[0];    // OK
    //let v1: i32 = arr[x];    // ERROR NameRes: 'x' no está declarado
    return ();
}

// 10. Printstmt (NameRes solo resuelve 'msg')
fn test_println()  {
    let msg: &str = "hello";
    println!("{msg}", 1, 2);     // NameRes: enlaza 'msg'; aridad/tipos van en TypeChecker
    return ();
}

// 1. Asignación válida a arr[1]
fn test_slice_assign_ok()  {
    let mut arr: [i32; 3] = [0, 1, 2];
    arr[1] = 42;       // OK: 'arr' existe, índice literal                     esto deberia funcionar o no?
    return ();
}

// 2. Asignación a arr[0] cuando 'arr' no existe
fn test_slice_assign_undef_arr()  {
    //arr[0] = 5;        // ERROR NameRes: undefined identifier 'arr'
    return ();
}

// 3. Asignación a arr[i] cuando 'i' no existe
fn test_slice_assign_undef_index()  {
    let arr: [i32; 3] = [0, 1, 2];
   // arr[i] = 7;        // ERROR NameRes: undefined identifier 'i'
    return ();
}

// 4. Asignación arr[j] = … con ambos 'arr' y 'j' indefinidos
fn test_slice_assign_both_undef()  {
    //arr[j] = 9;        // ERROR NameRes: undefined identifier 'arr'
    // (y si 'arr' existiera, luego 'j' fallaría)
    return ();
}

// 5. Lectura válida de arr[2]
fn test_slice_read_ok()  {
    let arr: [i32; 3] = [10, 20, 30];
    let x: i32 = arr[2];  // OK: arr y 2 bien resueltos
    return ();
}

// 6. Lectura arr[k] en declaración, 'k' indefinido
fn test_slice_read_undef_index()  {
    let arr: [i32; 3] = [10, 20, 30];
    //let y: i32 = arr[k];  // ERROR NameRes: undefined identifier 'k'
    return ();
}

// 7. Lectura de arr2[0] cuando 'arr2' no existe
fn test_slice_read_undef_arr()  {
    //let z: i32 = arr2[0]; // ERROR NameRes: undefined identifier 'arr2'
    return ();
}

// 1. Slice válido sobre variable declarada
fn test_slice_ok()  {
    let s: &str = "abcd";
    let t: &str = s[1..3];   // OK: 's' existe, 1 y 3 son literales
    return ();
}

// 2. Slice sobre variable no declarada
fn test_slice_undef_var()  {
    //let t: &str = u[0..2];   // ERROR NameRes: undefined identifier 'u'
    return ();
}

// 3. Slice con índice de inicio indefinido
fn test_slice_undef_start()  {
    let s: &str = "hola";
    //let x: &str = s[a..2];   // ERROR NameRes: undefined identifier 'a'
    return ();
}

// 4. Slice con índice final indefinido
fn test_slice_undef_end()  {
    let s: &str = "mundo";
    //let y: &str = s[1..b];   // ERROR NameRes: undefined identifier 'b'
    return ();
}

// 5. Slice con ambos índices indefinidos
fn test_slice_both_undef()  {
    //let z: &str = v[i..j];   // ERROR NameRes: undefined identifier 'v'
    // (si v existiera, luego i ó j fallaría)
    return ();
}

// 6. Slice sobre literal (no hay lookup de variable)
fn test_slice_literal()  {
    //let r: &str = "rust"[2..4]; // OK: no lookup de variable, solo literales
    return ();
}

// 7. Reasignación usando slice (NameRes enlaza 's' como variable)
fn test_slice_assign()  {
    let mut s: &str = "hello";
    s = s[1..3];              // OK NameRes: 's' existe, 1 y 3 literales
    return ();
}
