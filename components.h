String dashboard = R"==(<nav class="navbar">
        <div class="navbar__container">
          <a href="/" id="navbar__logo">NEXT</a>
          <div class="navbar__toggle" id="mobile-menu">
            <span class="bar"></span>
            <span class="bar"></span>
            <span class="bar"></span>
          </div>
          <ul class="navbar__menu">
            <li class="navbar__item">
                <a href="index.html" class="navbar__links">Home</a>
            </li>
            <li class="navbar__item">
                <a href="settings.html" class="navbar__links">Settings</a>
            </li>
            <li class="navbar__item">
                <a href="/" class="navbar__links">Products</a>
            </li>
            <li class="navbar__btn"><a href="/" 
                class="button">Sign up</a></li>
          </ul>
        </div>
    </nav>
    <div margin-top="50px" id="settings_div">
        <h1 style="text-align:center" id="settings_title">Settings</h1>
        <label for="sensor_dist">Sensor Distance: </label>
        <input type="range" min="1" max="100" id="sensor_dist" value="50">
        <label for="sensor_dist" id="sensor_dist_display"></label>
    </br>
        <label for="vibrate_strength">Vibrate Strength: </label>
        <input type="range" min="0.01" max="1" id="vibrate_strength" value="1" step="0.01">
        <label for="vibrate_strength" id="vibrate_strength_display"></label>
    </div>==)";