String dashboard = R"==(<nav class="navbar">

        <div class="navbar__container">
          <a href="/" id="navbar__logo">Object Sensor</a>
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
          </ul>
        </div>
    </nav>
    )==";

String settings = R"==(
<div margin-top="50px" id="settings_div">
        <h1 style="text-align:center" id="settings_title">Settings</h1>
        <label for="sensor_dist">Sensor Distance: </label>
        <input type="range" min="1" max="100" id="sensor_dist" value="50">
        <label for="sensor_dist" id="sensor_dist_display"></label>
    </br>
        <label for="vibrate_strength">Vibrate Strength: </label>
        <input type="range" min="0.01" max="1" id="vibrate_strength" value="1" step="0.01">
        <label for="vibrate_strength" id="vibrate_strength_display"></label>
    </div>
)==";

String dashboardCSS = R"==(*{
    box-sizing: border-box;
    margin: 0;
    padding: 0;
    font-family:'Kumbh Sans', sans-serif;
}

body {
    background: rgba(2,5,5,0.3);
}


.navbar{
    background: #131313;
    height: 80px;
    display: flex;
    justify-content: center;
    align-items: center;
    font-size: 1.2rem;
    position: sticky;
    top: 0;
    z-index: 999;
}



.navbar__container{
    display: flex;
    justify-content: space-between;
    height: 80px;
    z-index: 1;
    width: 100%;
    max-width: 1300px;
    margin: 0 auto;
    padding: 0 50px;
}

#navbar__logo{
    background-color: #ff1877;
    background-image: linear-gradient(to top, #ff0844, #ffb199
    100%);
    -webkit-background-clip: text;
    -moz-background-clip: text;
    -webkit-text-fill-color: transparent;
    -moz-text-fill-color: transparent;
    -moz-text-fill-color: transparent;
    display: flex;
    align-items: center;
    cursor: pointer;
    text-decoration: none;
    font-size: 2rem;
}


.fa-gem {
    margin-right: flex;
}

.navbar__menu {
    display: flex;
    align-items: cenetr;
    list-style: none;
    text-align: center;
}


.navbar__item{
    height: 80px;
}


.navbar__links {
    color: #fff;
    display: flex;
    align-items: center;
    justify-content: center;
    text-decoration: none;
    padding: 0 1rem;
    height: 100%;
}

.navbar__btn{
    display: flex;
    justify-content: center;
    align-items: center;
    padding: 0 1rem;
    width: 100%;
}

.button {
    display: flex;
    justify-content: center;
    align-items: center;
    text-decoration: none;
    padding: 10px 20px;
    height: 100%;
    width: 100%;
    border: none;
    outline: none;
    border-radius: 4px;
    background: #f77062;
    color: #fff;

}

.button:hover{
    background: #4837ff;
    transform: all 0.3s ease;

}


.navbar__links:hover{
    color: #f77062;
    transform: all 0.3s ease;
}

@media screen and(max-width: 960px){
    .navbar__container{
        display: flex;
        justify-content: space-between;
        height: 80px;
        z-index: 1;
        width: 100%;
        max-width: 1300px;
        padding: 0;
    }


    .navbar__menu{
        display: grid;
        grid-template-columns: auto;
        margin: 0;
        width: 100%;
        position: absolute;
        /* top: -1000px;
        opacity: 0; */
        transition: all 0.5s ease;
        height: 50vh;
        z-index: -1;
    }

    .navbar__menu.active{
        background: #131313;
        top: 100%;
        opacity: 1;
        transition: all 0.5s ease;
        z-index: 99;
        height: 50vh;
        font-size: 1.6rem;
    }
    
    #navbar__logo{
        padding-left: 25px;
    }

    .navbar__toggle .bar{
        width: 25px;
        height: 3px;
        margin: 5px auto;
        transition: all 0.3s ease-in-out;
        background: #fff;
    }

    .navbar__item{
        width: 100%;
    }

    .navbar__links {
        text-align: center;
        padding: 2rem;
        width: 100%;
display: table;
    }

    #mobile-menu{
        position: absolute;
        top: 20%;
        right: 5%;
        transform: translate(5%, 20%);

    }

    .navbar__btn{
        padding-bottom: 2rem;

    }

    .button {
        display: flex;
        justify-content: center;
        
    }
}

#settings_div {
    margin-top: 35px;
    margin-left: 35%;
    margin-right: 35%;
}

#settings_title{
    margin-bottom: 20px;
})==";

String speedometerJS = R"==(
<script>
      $(document).ready(function() {
            let speed = 0;
            let maxSpeed = 200;

            setInterval(function() {
                speed += Math.floor(Math.random() * 20) + 1;
                if (speed > maxSpeed) {
                    speed = maxSpeed;
                }

                let rotation = (speed / maxSpeed) * 180;
                $('.needle').css('transform', 'translate(-50%, 0) rotate(' + rotation + 'deg)');
                $('.speed').text(speed);
            }, 1000);
        });
    </script>
)==";

String speedometerCSS = R"==(
      #speedometer {
            width: 200px;
            height: 200px;
            border-radius: 50%;
            background-color: #f5f5f5;
            display: flex;
            justify-content: center;
            align-items: center;
            position: relative;
      }
 
      #speedometer .needle {
            width: 2px;
            height: 80px;
            background-color: red;
            position: absolute;
            bottom: 50%;
            left: 50%;
            transform-origin: bottom center;
            transform: translate(-50%, 0) rotate(0deg);
            transition: transform 0.5s;
      }
 
      #speedometer .speed {
            font-size: 24px;
            font-weight: bold;
            margin-top: 20px;
      }
)==";

String speedometer() {
  return String(R"==(
  <!-- Speedometer -->
  <div class="text-center" id="speedometer">
   <div class="needle">
   </div>
   <div class="speed">
    )==") + String(distance) + String(R"==(
   </div>
)==");
}
