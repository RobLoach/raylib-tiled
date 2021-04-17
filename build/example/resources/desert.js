(function(name,data){
 if(typeof onTileMapLoaded === 'undefined') {
  if(typeof TileMaps === 'undefined') TileMaps = {};
  TileMaps[name] = data;
 } else {
  onTileMapLoaded(name,data);
 }
 if(typeof module === 'object' && module && module.exports) {
  module.exports = data;
 }})("desert",
{ "compressionlevel":-1,
 "editorsettings":
    {
     "export":
        {
         "format":"js",
         "target":"desert.js"
        }
    },
 "height":40,
 "infinite":false,
 "layers":[
        {
         "compression":"zlib",
         "data":"eJztmNkKwjAQRaN9cAPrAq5Yq3Xf6v9\/nSM2VIbQJjEZR+nDwQZScrwztoORECLySBcIgZ7nc2y4KfyWDLx+Jb9nViNgDEwY+KioAXUgQN4+zpoCMwPmQAtoAx2CLFbA2oDEo9+hwG8DnIDtF\/2K8ks086Tw2zH0uyMv7HcRr\/6\/EvvhnsPrsrxwX7rwU\/0ODig\/eV3mh3N1ld8eraWPaX6+64s9McesfrqcHfg1MpoifxcVEWjukyw+9AtFPl\/I71pER3Of6j4bv7HI54s+MChhqLlPdZ\/P3qMmFuo5h5NnTOhjM5tReN2yT51n5\/v7J3F0vi46fk+ne7aX0i9l6If7mpufTX3f5wsqv9TAD2fJLT9VrTn7UeZnM5tR+v0LMQOHXwFnxe2\/warGFRWf8QDjOLfP",
         "encoding":"base64",
         "height":40,
         "id":1,
         "name":"Ground",
         "opacity":1,
         "type":"tilelayer",
         "visible":true,
         "width":40,
         "x":0,
         "y":0
        }],
 "nextlayerid":2,
 "nextobjectid":1,
 "orientation":"orthogonal",
 "renderorder":"right-down",
 "tiledversion":"1.5.0",
 "tileheight":32,
 "tilesets":[
        {
         "columns":8,
         "firstgid":1,
         "image":"tmw_desert_spacing.png",
         "imageheight":199,
         "imagewidth":265,
         "margin":1,
         "name":"Desert",
         "spacing":1,
         "tilecount":48,
         "tileheight":32,
         "tiles":[
                {
                 "id":30,
                 "probability":0.01
                }, 
                {
                 "id":31,
                 "probability":0.01
                }, 
                {
                 "id":37,
                 "probability":0.01
                }, 
                {
                 "id":38,
                 "probability":0.01
                }, 
                {
                 "id":39,
                 "probability":0.01
                }, 
                {
                 "id":45,
                 "probability":0
                }, 
                {
                 "id":46,
                 "probability":0.01
                }, 
                {
                 "id":47,
                 "probability":0.01
                }],
         "tilewidth":32,
         "wangsets":[
                {
                 "colors":[
                        {
                         "color":"#ff0000",
                         "name":"Desert",
                         "probability":1,
                         "tile":29
                        }, 
                        {
                         "color":"#00ff00",
                         "name":"Brick",
                         "probability":1,
                         "tile":9
                        }, 
                        {
                         "color":"#0000ff",
                         "name":"Cobblestone",
                         "probability":1,
                         "tile":33
                        }, 
                        {
                         "color":"#ff7700",
                         "name":"Dirt",
                         "probability":1,
                         "tile":14
                        }],
                 "name":"Desert",
                 "tile":5,
                 "type":"corner",
                 "wangtiles":[
                        {
                         "tileid":0,
                         "wangid":[0, 1, 0, 2, 0, 1, 0, 1]
                        }, 
                        {
                         "tileid":1,
                         "wangid":[0, 1, 0, 2, 0, 2, 0, 1]
                        }, 
                        {
                         "tileid":2,
                         "wangid":[0, 1, 0, 1, 0, 2, 0, 1]
                        }, 
                        {
                         "tileid":3,
                         "wangid":[0, 4, 0, 1, 0, 4, 0, 4]
                        }, 
                        {
                         "tileid":4,
                         "wangid":[0, 4, 0, 4, 0, 1, 0, 4]
                        }, 
                        {
                         "tileid":5,
                         "wangid":[0, 1, 0, 4, 0, 1, 0, 1]
                        }, 
                        {
                         "tileid":6,
                         "wangid":[0, 1, 0, 4, 0, 4, 0, 1]
                        }, 
                        {
                         "tileid":7,
                         "wangid":[0, 1, 0, 1, 0, 4, 0, 1]
                        }, 
                        {
                         "tileid":8,
                         "wangid":[0, 2, 0, 2, 0, 1, 0, 1]
                        }, 
                        {
                         "tileid":9,
                         "wangid":[0, 2, 0, 2, 0, 2, 0, 2]
                        }, 
                        {
                         "tileid":10,
                         "wangid":[0, 1, 0, 1, 0, 2, 0, 2]
                        }, 
                        {
                         "tileid":11,
                         "wangid":[0, 1, 0, 4, 0, 4, 0, 4]
                        }, 
                        {
                         "tileid":12,
                         "wangid":[0, 4, 0, 4, 0, 4, 0, 1]
                        }, 
                        {
                         "tileid":13,
                         "wangid":[0, 4, 0, 4, 0, 1, 0, 1]
                        }, 
                        {
                         "tileid":14,
                         "wangid":[0, 4, 0, 4, 0, 4, 0, 4]
                        }, 
                        {
                         "tileid":15,
                         "wangid":[0, 1, 0, 1, 0, 4, 0, 4]
                        }, 
                        {
                         "tileid":16,
                         "wangid":[0, 2, 0, 1, 0, 1, 0, 1]
                        }, 
                        {
                         "tileid":17,
                         "wangid":[0, 2, 0, 1, 0, 1, 0, 2]
                        }, 
                        {
                         "tileid":18,
                         "wangid":[0, 1, 0, 1, 0, 1, 0, 2]
                        }, 
                        {
                         "tileid":19,
                         "wangid":[0, 2, 0, 1, 0, 2, 0, 2]
                        }, 
                        {
                         "tileid":20,
                         "wangid":[0, 2, 0, 2, 0, 1, 0, 2]
                        }, 
                        {
                         "tileid":21,
                         "wangid":[0, 4, 0, 1, 0, 1, 0, 1]
                        }, 
                        {
                         "tileid":22,
                         "wangid":[0, 4, 0, 1, 0, 1, 0, 4]
                        }, 
                        {
                         "tileid":23,
                         "wangid":[0, 1, 0, 1, 0, 1, 0, 4]
                        }, 
                        {
                         "tileid":24,
                         "wangid":[0, 1, 0, 3, 0, 1, 0, 1]
                        }, 
                        {
                         "tileid":25,
                         "wangid":[0, 1, 0, 3, 0, 3, 0, 1]
                        }, 
                        {
                         "tileid":26,
                         "wangid":[0, 1, 0, 1, 0, 3, 0, 1]
                        }, 
                        {
                         "tileid":27,
                         "wangid":[0, 1, 0, 2, 0, 2, 0, 2]
                        }, 
                        {
                         "tileid":28,
                         "wangid":[0, 2, 0, 2, 0, 2, 0, 1]
                        }, 
                        {
                         "tileid":29,
                         "wangid":[0, 1, 0, 1, 0, 1, 0, 1]
                        }, 
                        {
                         "tileid":30,
                         "wangid":[0, 1, 0, 1, 0, 1, 0, 1]
                        }, 
                        {
                         "tileid":31,
                         "wangid":[0, 1, 0, 1, 0, 1, 0, 1]
                        }, 
                        {
                         "tileid":32,
                         "wangid":[0, 3, 0, 3, 0, 1, 0, 1]
                        }, 
                        {
                         "tileid":33,
                         "wangid":[0, 3, 0, 3, 0, 3, 0, 3]
                        }, 
                        {
                         "tileid":34,
                         "wangid":[0, 1, 0, 1, 0, 3, 0, 3]
                        }, 
                        {
                         "tileid":35,
                         "wangid":[0, 3, 0, 1, 0, 3, 0, 3]
                        }, 
                        {
                         "tileid":36,
                         "wangid":[0, 3, 0, 3, 0, 1, 0, 3]
                        }, 
                        {
                         "tileid":37,
                         "wangid":[0, 1, 0, 1, 0, 1, 0, 1]
                        }, 
                        {
                         "tileid":38,
                         "wangid":[0, 1, 0, 1, 0, 1, 0, 1]
                        }, 
                        {
                         "tileid":39,
                         "wangid":[0, 1, 0, 1, 0, 1, 0, 1]
                        }, 
                        {
                         "tileid":40,
                         "wangid":[0, 3, 0, 1, 0, 1, 0, 1]
                        }, 
                        {
                         "tileid":41,
                         "wangid":[0, 3, 0, 1, 0, 1, 0, 3]
                        }, 
                        {
                         "tileid":42,
                         "wangid":[0, 1, 0, 1, 0, 1, 0, 3]
                        }, 
                        {
                         "tileid":43,
                         "wangid":[0, 1, 0, 3, 0, 3, 0, 3]
                        }, 
                        {
                         "tileid":44,
                         "wangid":[0, 3, 0, 3, 0, 3, 0, 1]
                        }, 
                        {
                         "tileid":45,
                         "wangid":[0, 1, 0, 1, 0, 1, 0, 1]
                        }, 
                        {
                         "tileid":46,
                         "wangid":[0, 1, 0, 1, 0, 1, 0, 1]
                        }, 
                        {
                         "tileid":47,
                         "wangid":[0, 1, 0, 1, 0, 1, 0, 1]
                        }]
                }]
        }],
 "tilewidth":32,
 "type":"map",
 "version":1.5,
 "width":40
});