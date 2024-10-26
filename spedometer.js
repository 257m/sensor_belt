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