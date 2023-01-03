const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
    <head>
        <style>
            :root {
                --scale: calc(min(14vw, 6vh));
            }

            main {
                all: none;
            }
            body {
                text-align:center;
                font-family: Cambria, Cochin, Georgia, Times, 'Times New Roman', serif;
                background-color: black;
                color: white;
                font-weight: 200;
                font-size: calc(var(--scale) / 3);
                letter-spacing: 1px;
            }
            main {
                margin: 0;
                position: absolute;
                top: 50%;
                left: 50%;
                -ms-transform: translate(-50%, -50%);
                transform: translate(-50%, -50%);
            }
            .title {
                font-weight: 900;
                font-size: var(--scale);
            }
            svg {
                width: calc(var(--scale) * 7 / 5);
                height: calc(var(--scale) * 6 / 5);
            }
            section {
                margin-top: var(--scale);
            }
            button {
                all: unset;
            }
            input {
                all: unset;
                text-align: left;
                width: 100%;
            }
            select {
                all: unset;
                text-align: left;
                width: 100%;
            }
            .btn {
            margin-top: calc(var(--scale) * 3 / 5);
            line-height: var(--scale);
            height: var(--scale);
            text-align: center;
            width: calc(var(--scale) * 5);
            cursor: pointer;
            }
            .btn-one {
            color: #FFF;
            transition: all 0.3s;
            position: relative;
            }
            .btn-one span {
            transition: all 0.3s;
            }
            .btn-one::before {
            content: '';
            position: absolute;
            bottom: 0;
            left: 0;
            width: 100%;
            height: 100%;
            z-index: 1;
            opacity: 0;
            transition: all 0.3s;
            border-top-width: 1px;
            border-bottom-width: 1px;
            border-top-style: solid;
            border-bottom-style: solid;
            transform: scale(0.1, 1);
            }
            .btn-one:hover span {
            letter-spacing: calc(var(--scale) * 2 / 5);
            }
            .btn-one:hover::before {
            opacity: 1; 
            transform: scale(1, 1); 
            }
            .btn-one::after {
            content: '';
            position: absolute;
            bottom: 0;
            left: 0;
            width: 100%;
            height: 100%;
            z-index: 1;
            transition: all 0.3s;
            }
            .btn-one:hover::after {
            opacity: 0; 
            transform: scale(0.1, 1);
            }
        </style>
    </head>
    <body>
        <main>
            <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 971.42 758.65"><defs><style>.cls-1{fill:#fff;}</style></defs><g id="Layer_2" data-name="Layer 2"><g id="Layer_3" data-name="Layer 3"><path class="cls-1" d="M970,260.14a21.42,21.42,0,0,0-21-19.36,21.71,21.71,0,0,0-17.1,8.77,20.24,20.24,0,0,0-3.94,13c-12.13,3.21-24.46,5.91-36.85,8,14.4-31.9,18.82-70.08,11.75-104.41a6,6,0,0,0-5.88-4.79h0a6,6,0,0,0-5.87,4.83A159.9,159.9,0,0,1,808.77,276.8,430.1,430.1,0,0,1,703.2,261.54c27.95-16.49,51.14-41.88,66.2-72.76,17.64-36.16,22.4-76.63,13.41-113.94a6,6,0,0,0-5.82-4.6c-.22,0-.44,0-.67,0A172.81,172.81,0,0,0,623.11,231.33a394.59,394.59,0,0,0-38.44-22.62L652.81,62.1l39.94,2.32,2.79-47.92L425.15.78A460.53,460.53,0,0,0,0,229.7l41.52,24.09q8.81-15.18,18.71-29.38L87.15,274.6a385.6,385.6,0,0,0-44.41,47.89c-18,23-27.05,47.3-27.05,72.08a109.51,109.51,0,0,0,89.45,107.84,9.13,9.13,0,0,1-1.82,18.1,8.66,8.66,0,0,1-7-3.45c-9-11.44-21.89-18-35.23-18l-1.11,0c-30.22.8-44.58,23-45.13,43.32S27,585.81,56.2,590.1a96.88,96.88,0,0,1,3.13,24.53,97.11,97.11,0,0,1-22.67,62.3,6,6,0,0,0-1.3,5,431,431,0,0,1,7.28,70.88,6,6,0,0,0,6,5.88h68.75a6,6,0,0,0,5.87-7.27c-12.66-58.9-7.34-115.09,15.82-167,36.76-11.83,70.18-37.32,92-70.26,18.35-27.67,37.2-73.75,22.82-136.14-1.44-6.24-.45-11.73,2.86-15.9a20.24,20.24,0,0,1,15.6-7.09,19,19,0,0,1,6.95,1.27c166.2,65.43,275.09,221.07,277.42,396.51a6,6,0,0,0,6,5.92H658.9a6,6,0,0,0,5.15-9.08,260.92,260.92,0,0,1-34-94A69.66,69.66,0,0,0,618.91,587c.18-1.17.42-2.69.71-4.49l2.76-17.64c3.4-21.7,8.52-54.32,9.12-57.48,0-.15.07-.31.1-.47,4.56-27,28.23-46.91,56.27-47.41,96.37-1.71,191.58-37.69,268.11-101.3a6.2,6.2,0,0,0,1.64-2.16C969.57,329.37,974,295.3,970,260.14Zm-859.41-5.32L80.16,198.09q11.37-13.83,23.77-26.53l29.91,66.36Q121.89,246,110.58,254.82Zm48.94-32.94L127,149.7a411.22,411.22,0,0,1,33-26.06l26.31,84.05Q172.59,214.34,159.52,221.88Zm54.35-26.45-27.81-88.86a408.77,408.77,0,0,1,44.19-23L244.32,184.4Q228.87,189.38,213.87,195.43Zm59.58-19.38L258.93,72a410.27,410.27,0,0,1,52.24-14.87L307,168.9Q290.09,171.86,273.45,176.05Zm63.75-11.41,4.19-112.78A414.7,414.7,0,0,1,393.86,48L372.2,162.2Q354.67,162.76,337.2,164.64Zm65.53-2.41L424.24,48.81,472,51.59l-35,113.13Q420,162.85,402.73,162.23Zm64.38,6.87L502.86,53.38,547.19,56,499.3,176.19Q483.39,172,467.11,169.1ZM557.51,196q-14.38-6.12-29.36-11.16l50.6-127,41.84,2.44Z"/></g></g></svg>
            <div class="title">RAYONX</div>
            <div>PAR TOUTATIX</div>
            <section>
                <form method="POST">
                    <div>WIFI'S NAME</div>
                    <p>
                        <select id="ssid" name="ssid">
                            *option*
                        </select>
                    </p>
                    <div>PASSWORD</div>
                    <p>
                        <input type="password" name="password" value="*password*" placeholder="Password">
                    </p>
                    <button class="btn btn-one" type="submit">CONNECT</button>
                </form>
            </section>
        </main>
    </body>
</html>
)=====";
