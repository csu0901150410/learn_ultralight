
// js注册到c++的的函数列表
let dynamic_function_array = [];

// c++调用js函数的统一入口
function call_to_js(name, params)
{
    call_dynamic_function(name, params);
}

function register_dynamic_function(name, func)
{
    dynamic_function_array[name] = func;
}

function unregister_dynamic_function(name)
{
    delete dynamic_function_array[name];
}

function call_dynamic_function(name, params)
{
    if (dynamic_function_array[name])
    {
        dynamic_function_array[name](params);
    }
}

function dynamic_function_1(message)
{
    document.getElementById('msg').innerHTML = message;
}

function dynamic_function_2(message)
{
    document.getElementById('result').innerHTML = message;
}

// js调用c++
function OnButtonClick()
{
    expose_to_js("command_00");
}

// 绑定界面回调函数，也就是js调用c++函数
function bindCallbacks()
{
    document.querySelector('#btn').addEventListener('click', event => OnButtonClick());
}

function on_dom_load()
{
    bindCallbacks();

    register_dynamic_function("dynamic_function_1", dynamic_function_1);
    register_dynamic_function("dynamic_function_2", dynamic_function_2);
}

// 文档加载完成时调用on_dom_load函数
document.addEventListener('DOMContentLoaded', on_dom_load);
