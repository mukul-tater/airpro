$(document).ready(function () {
  $('.sidebar-nav li a').click(function(e) {

      $('.sidebar-nav li.active').removeClass('active');

      var $parent = $(this).parent();
      $parent.addClass('active');
      e.preventDefault();
  });

  $('.pm').click(function() {
      $('#pm').toggle('slow');
  });

  $('.smoke').click(function() {
      $('#smoke').toggle('slow');
  });

  $('.co').click(function() {
      $('#co').toggle('slow');
  });

  $('.nh3').click(function() {
      $('#nh3').toggle('slow');
  });

var MONTHS = ["January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"];
    var config1 = {
        type: 'line',
        data: {
            labels: ["Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"],
            datasets: [{
                label: "AQI",
                backgroundColor: 'rgba(222, 98, 98, 0.2)',
                borderColor: "#DE6262",
                data: [randomScalingFactor(),
                        randomScalingFactor(),
                        randomScalingFactor(),
                        randomScalingFactor(),
                        randomScalingFactor(),
                        randomScalingFactor(),
                        randomScalingFactor()],
                fill: 'origin',
                pointRadius: 5,
                pointBorderWidth: 4,
                pointBackgroundColor: "#fff",
            }]
        },
        options: {
            responsive: true,
            title:{
                display:true,
                text:'Air Quality Index'
            },
            tooltips: {
                mode: 'index',
                intersect: false,
            },
            hover: {
                mode: 'nearest',
                intersect: true
            },
            scales: {
                xAxes: [{
                    display: true,
                    scaleLabel: {
                        display: true,
                        labelString: 'Day'
                    }
                }],
                yAxes: [{
                    display: true,
                    scaleLabel: {
                        display: true,
                        labelString: 'Value'
                    }
                }]
            }
        }
    };

    var colorNames = Object.keys(window.chartColors);

    // PIE CHART

    var config2 = {
        type: 'doughnut',
        data: {
            datasets: [{
                data: [20, 10,10, 30, 20],
                backgroundColor: ['#DE6262', '#5FABDF', '#70C67F', '#9D61DD', '#636FDF' ],
                label: 'Dataset 1'
            }],
            labels: [
                "PM 2.5",
                "NO2,CO2,NH3",
                "CO",
                "Smoke",
                "Humidity"
            ]
        },
        options: {
            responsive: true
        }
    };

    window.onload = function() {
      var ctx = document.getElementById("line-chart").getContext("2d");
      window.myLine = new Chart(ctx, config1);

        var ctx2 = document.getElementById("pie-chart").getContext("2d");
        window.myPie = new Chart(ctx2, config2);
    };

});
