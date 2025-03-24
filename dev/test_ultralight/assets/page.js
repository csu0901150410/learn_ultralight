// js调用c++
function OnButtonClick()
{
    // 把Home的字样改了
    document.querySelector('#btn').innerHTML = "Hello C++";

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
}

// 文档加载完成时调用on_dom_load函数
document.addEventListener('DOMContentLoaded', on_dom_load);