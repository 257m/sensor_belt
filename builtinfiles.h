/**
 * @file builtinfiles.h
 * @brief This file is part of the WebServer example for the ESP8266WebServer.
 *
 * This file contains long, multiline text variables for  all builtin resources.
 */

// used for $upload.htm
static const char uploadContent[] PROGMEM =
  R"==(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>First Website</title>
    <link rel="stylesheet" href="styles.css">
</head>
<body>
    <nav class="navbar">
        <div class="navbar__container">
          <a href="/" id="navbar__logo">NEXT</a>
          <div class="navbar__toggle" id="mobile-menu">
            <span class="bar"></span>
            <span class="bar"></span>
            <span class="bar"></span>
          </div>
          <ul class="navbar__menu">
            <li class="navbar__item">
                <a href="/" class="navbar__links">Home</a>
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
</body>
</html>
)==";

// used for $upload.htm
static const char notFoundContent[] PROGMEM = R"==(
<html>
<head>
  <title>Resource not found</title>
</head>
<body>
  <p>The resource was not found.</p>
  <p><a href="/">Start again</a></p>
</body>
)==";
