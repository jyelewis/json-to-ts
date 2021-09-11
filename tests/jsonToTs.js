const childProcess = require("child_process");

exports.jsonToTs = async function jsonToTs(obj) {
    const json = JSON.stringify(obj);
    const child = childProcess.exec("../cmake-build-debug/json_to_ts", {
        cwd: __dirname
    });

    let output = '';

    child.stdout.setEncoding('utf8');
    child.stdout.on('data', function(data) {
        output += data.toString();
    });

    child.stdin.write(json);
    child.stdin.end();

    return new Promise((resolve, reject) => {
        child.on('close', () => {
            resolve(output);
        });
    });
}