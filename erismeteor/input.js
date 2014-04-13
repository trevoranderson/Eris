// Set up a collection to contain player information. On the server,
// it is backed by a MongoDB collection named "players".

Players = new Meteor.Collection("players");
Input = new Meteor.Collection("input");
LatestInputs = new Meteor.Collection("latest");
Screen = new Meteor.Collection("screen");

// val == up-1 down-2 left-3 right-4 start-5 select-6 a-7 b-8
Meteor.methods({ 
 
	stamp: function(val) {
		switch(val)
		{
			case 1:
				Input.insert(LatestInputs.findOne({}, {sort: {time: 1}}));
				LatestInputs.remove(LatestInputs.findOne({}, {sort: {time: 1}}));
				LatestInputs.insert({move: "up", time: (new Date()).getTime()}); 
				break;
			case 2:
                Input.insert(LatestInputs.findOne({}, {sort: {time: 1}}));
                LatestInputs.remove(LatestInputs.findOne({}, {sort: {time: 1}}));
				LatestInputs.insert({move: "down", time: (new Date()).getTime()});
				break;
			case 3:
                Input.insert(LatestInputs.findOne({}, {sort: {time: 1}}));
                LatestInputs.remove(LatestInputs.findOne({}, {sort: {time: 1}}));
				LatestInputs.insert({move: "left", time: (new Date()).getTime()});
				break;
			case 4:
                Input.insert(LatestInputs.findOne({}, {sort: {time: 1}}));
                LatestInputs.remove(LatestInputs.findOne({}, {sort: {time: 1}}));
				LatestInputs.insert({move: "right", time: (new Date()).getTime()});
				break;
			case 5:
                Input.insert(LatestInputs.findOne({}, {sort: {time: 1}}));
                LatestInputs.remove(LatestInputs.findOne({}, {sort: {time: 1}}));
				LatestInputs.insert({move: "start", time: (new Date()).getTime()});
				break;
			case 6:
                Input.insert(LatestInputs.findOne({}, {sort: {time: 1}}));
                LatestInputs.remove(LatestInputs.findOne({}, {sort: {time: 1}}));
				LatestInputs.insert({move: "select", time: (new Date()).getTime()});
				break;
			case 7:
                Input.insert(LatestInputs.findOne({}, {sort: {time: 1}}));
                LatestInputs.remove(LatestInputs.findOne({}, {sort: {time: 1}}));
				LatestInputs.insert({move: "a", time: (new Date()).getTime()});
				break;
			case 8:
                Input.insert(LatestInputs.findOne({}, {sort: {time: 1}}));
                LatestInputs.remove(LatestInputs.findOne({}, {sort: {time: 1}}));
				LatestInputs.insert({move: "b", time: (new Date()).getTime()});
				break;
			default:
		} 
		return (new Date()).getTime();
	}
});

if (Meteor.isClient) {
	
	Template.displayoverlay.latest = function() {
		return LatestInputs.find({}, {sort: {time: 1, move: -1}});
	};
	
	Template.outputinput.events({
	  
      'click #up': function () {
      	  Meteor.call('stamp', 1, function(error) {
			if (error)       
			 return alert(error.reason)
		  });
	  },
      'click #down': function () {
          Meteor.call('stamp', 2, function(error) {
            if (error)
             return alert(error.reason)
          });      
	  },
      'click #left': function () {
          Meteor.call('stamp', 3, function(error) {
            if (error)
             return alert(error.reason)
          });      
	  },
      'click #right': function () {
          Meteor.call('stamp', 4, function(error) {
            if (error)
             return alert(error.reason)
          });
      },
      'click #start': function () {
          Meteor.call('stamp', 5, function(error) {
            if (error)
             return alert(error.reason)
          });
      },
      'click #select': function () {
          Meteor.call('stamp', 6, function(error) {
            if (error)
             return alert(error.reason)
          });
      },
      'click #a': function () {
          Meteor.call('stamp', 7, function(error) {
            if (error)
             return alert(error.reason)
          });
      },
      'click #b': function () {
          Meteor.call('stamp', 8, function(error) {
            if (error)
             return alert(error.reason)
          });
      }
	});
  
  Template.renderboard.pixel = function () {
 	return Screen.find({}, {sort:{col: 1}});
  };  

  Template.renderboard.makePixel = function () {
	var div = document.createElement('div');
	div.className = 'pixel';
	div.style.backgroundColor = 'green';
	document.getElementById('game').appendchild(div);
  };
 
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
			for(var j = 0; j < moves.length; j++)	{
				Input.insert({move: moves[j], time: 0});
			}
		}

  	if (LatestInputs.find().count() === 0) {
	 	 var moves = ["Imagine", "How", "Much", "Better", "This", "Would", "Be", "With", "Internet", "._."];
   	 for(var j = 0; j < moves.length; j++) {
   	   LatestInputs.insert({move: moves[j], time: j});
   	 }
  	}
		if (Screen.find().count() === 0) {
			for (var i = 0; i<16; i++) {
				for (var j = 0; j<14; j++) {
					Screen.insert({row: i, col: j, color: Math.floor((i*j)%4)+1});
				}
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

		collectionApi = new CollectionAPI({ authToken: '97f0ad9e24ca5e0408a269748d7fe0a0' });
    collectionApi.addCollection(Players, 'players');
    collectionApi.addCollection(LatestInputs, 'latest');
    collectionApi.start();
  });
}
