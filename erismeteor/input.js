// Set up a collection to contain player information. On the server,
// it is backed by a MongoDB collection named "players".

Players = new Meteor.Collection("players");
Input = new Meteor.Collection("input");

if (Meteor.isClient) {
	
	Template.outputinput.input = function() {
	  return Input.find({}, {sort: {time: 1, move: -1}}); 
	};
	
	Template.outputinput.events({
	
      'click #up': function () {
      	 Input.insert({move: "up", time: (new Date()).getTime()});
	  },
      'click #down': function () {
         Input.insert({move: "down", time: (new Date()).getTime()});
      },
      'click #right': function () {
         Input.insert({move: "right", time: (new Date()).getTime()});
      },
      'click #left': function () {
         Input.insert({move: "left", time: (new Date()).getTime()});
      },
      'click #start': function () {
         Input.insert({move: "start", time: (new Date()).getTime()});
      },
      'click #select': function () {
         Input.insert({move: "select", time: (new Date()).getTime()});
      },
      'click #a': function () {
         Input.insert({move: "a", time: (new Date()).getTime()});
      },
      'click #b': function () {
         Input.insert({move: "b", time: (new Date()).getTime()});
      }
	});

  Template.leaderboard.players = function () {
    return Players.find({}, {sort: {score: -1, name: 1}});
  };
	
  Template.leaderboard.selected_name = function () {
    var player = Players.findOne(Session.get("selected_player"));
    return player && player.name;
  };

  Template.player.selected = function () {
    return Session.equals("selected_player", this._id) ? "selected" : '';
  };

  Template.leaderboard.events({
    'click input.inc': function () {
      Players.update(Session.get("selected_player"), {$inc: {score: 10}});
    },
	  'click .Dummy': function () {
      Players.update(Session.get("selected_player"), {$inc: {score: 5}});
	} 
  });

  Template.player.events({
    'click': function () {
      Session.set("selected_player", this._id);
    }
  });
}

// On server startup, create some players if the database is empty.
if (Meteor.isServer) {
  Meteor.startup(function () {
	if (Input.find().count() === 0){
		var moves = ["potato", "kitty", "stacks"];
		for(var j = 0; j < moves.length; j++)
		{
			Input.insert({move: moves[j], time: 0});
		}
	}

    if (Players.find().count() === 0) {
      var names = ["Ada Lovelace",
                   "Grace Hopper",
                   "Marie Curie",
                   "Carl Friedrich Gauss",
                   "Nikola Tesla",
                   "Claude Shannon"];
      for (var i = 0; i < names.length; i++)
        Players.insert({name: names[i], score: Math.floor(Random.fraction()*10)*5});
 	}
  });
}
