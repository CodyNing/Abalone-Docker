$a = document.querySelectorAll.bind(document);
$d = document.querySelector.bind(document);
$c = document.createElement.bind(document);

const ROW_NUMBER_MAP = [ 5, 6, 7, 8, 9, 8, 7, 6, 5 ];

class Slot {
    constructor(index, size = 10) {
        this.index = index;
        this.state = 'empty'
        this.html = document.createElement('div');
        this.html.style.height = size + 'vw';
        this.html.style.width = size + 'vw';
        this.html.classList.add('slot');
        let marble = document.createElement('div');
        marble.classList.add('marble');
        this.marble = marble;
        this.html.appendChild(marble);
    }

    setState() {
        this.html.classList.remove(this.state);
        this.marble.classList.remove(this.state);
        if (arguments.length > 0 && arguments[0] !== undefined) {
            this.html.classList.add(arguments[0]);
            this.state = arguments[0];
        } else {
            this.state = 'empty';
        }
    }

    select(isSelected) {
        this.isSelected = isSelected;
        if (isSelected) {
            this.html.classList.add('selected');
        } else {
            this.html.classList.remove('selected');
        }
    }

    click(fn) {
        this.html.addEventListener('click', e => fn(e));
    }
}


class GameBoard {
    constructor(size) {
        this.size = size;
        this.slotSize = Math.trunc(size / 9);
        this.slots = [];
        this.html = document.createElement('div');
        this.html.classList.add('game_board');
        this.html.style.height = size + 6 * this.slotSize + "vw";
        this.html.style.width = size + 6 * this.slotSize + "vw";
        let slotNo = 0;
        for (let i = 0; i < 9; ++i) {
            let rowSlotNum = ROW_NUMBER_MAP[i];
            let row = document.createElement('div');
            row.classList.add('game_board_row_' + i);
            // row.style.marginLeft = (9 - rowSlotNum) * (this.slotSize / 2) + 'vw';
            for (let j = 0; j < rowSlotNum; ++j) {
                let slot = new Slot(slotNo++, this.slotSize);
                this.slots.push(slot);
                row.appendChild(slot.html);
                slot.setState('white');
            }
            this.html.appendChild(row);
        }
    }

    selectSlots(list) {
        this.slots.forEach(s => s.select(false));
        list.forEach(i => this.slots[i].select(true));
    }

    unSelectAll(){
        this.slots.forEach(s => s.select(false));
    }

    setBoardState(slotStates){
        let i = 0
        let map = {
            0: 'empty',
            1: 'black',
            2: 'white'
        }

        slotStates.forEach(state=>{
            this.slots[i++].setState(map[state]);
        });
    }
}

class Game {
    constructor() { }

    static createCookie(name, value, days) {
        let expires = null;
        if (days) {
            let date = new Date();
            date.setTime(date.getTime() + (days * 24 * 60 * 60 * 1000));
            expires = "; expires=" + date.toGMTString();
        }
        else {
            expires = "";
        }
        document.cookie = name + "=" + value + expires + "; path=/";
    }

    static getCookie(c_name) {
        if (document.cookie.length > 0) {
            let c_start = document.cookie.indexOf(c_name + "=");
            if (c_start != -1) {
                c_start = c_start + c_name.length + 1;
                let c_end = document.cookie.indexOf(";", c_start);
                if (c_end == -1) {
                    c_end = document.cookie.length;
                }
                return unescape(document.cookie.substring(c_start, c_end));
            }
        }
        return "";
    }

    static delete_cookie(name) {
        document.cookie = name + '=; expires=Thu, 01 Jan 1970 00:00:01 GMT;';
    }

    static async gameMain() {
        let game = null;
        let token = Game.getCookie('mg_token');

        if (token === "") {
            game = await Game.newGame();
        } else {
            game = await Game.resumeGame(token);
        }
        $d('#terminate_btn').addEventListener('click', async e => {
            if (confirm("Are you sure you want to terminate?")) {
                await game.terminateGame();
                await Game.newGame();
            }
        });
        return game;
    }

    static async newGame() {
        $("#create_game_modal").modal({
            backdrop: 'static',
            keyboard: false
        });

        $d("#ai_time").addEventListener('change', e => {
            let val = e.target.value;
            if(!Number.isInteger(val)){
                e.target.value = Math.round(val);
                val = e.target.value;
            }
            if (val < 2) {
                e.target.value = 2;
            } else if (val > 30) {
                e.target.value = 30;
            }
        });

        $d("#max_move").addEventListener('change', e => {
            let val = e.target.value;
            if(!Number.isInteger(val)){
                e.target.value = Math.round(val);
                val = e.target.value;
            }
            if (val < 10) {
                e.target.value = 10;
            }
        });

        let game = new Game();

        await new Promise( res=>
            $d('#start_btn').addEventListener('click', async e=>{
                let aiTime = $d("#ai_time").value;
                let maxMove = $d("#max_move").value;
                let initState = 0;
                for (const ele of $a("input[name='init_state_radio']")){
                    if(ele.checked){
                        initState = ele.value;
                        break;
                    }
                }
                await game.gameInit(initState, maxMove, aiTime);
                $("#create_game_modal").modal('hide');
                res()
            })
        )
        return game;
    }

    static async resumeGame(token) {
        let game = new Game();
        try {
            let gameinfo = await Game.getGameinfo(token);
            game.updateGameinfo(gameinfo);
            await game.initNewBoard();
            return game;
        } catch (e) {
            Game.delete_cookie('mg_token');
            return await Game.newGame();
        }
    }

    static async getGameinfo(sessionId) {
        return await fetch("./Game/GetGame", {
            method: 'POST',
            headers: {
                'Accept': 'application/json',
                'Content-Type': 'application/json'
            },
            body: JSON.stringify({
                SessionId: sessionId
            })
        }).then(r => r.json());
    }

    async gameInit(initState, maxMove, aiTime) {
        let gameinfo = await this.requestNewGame(initState, maxMove, aiTime);
        Game.createCookie("mg_token", gameinfo.sessionId, 1);
        this.updateGameinfo(gameinfo);

        await this.initNewBoard();
    }

    async requestNewGame(initState, maxMove, aiTime) {
        return await fetch("./Game/CreateGame", {
            method: 'POST',
            headers: {
                'Accept': 'application/json',
                'Content-Type': 'application/json'
            },
            body: JSON.stringify({
                InitState: initState,
                AITimeSec: aiTime,  
                MoveLeft: maxMove
            }),
        }).then(r => r.json());
    }

    async gameSendAction(action, actionParam) {
        let res = await fetch("./send-game-action", {
            method: 'POST',
            headers: {
                'Accept': 'application/json',
                'Content-Type': 'application/json'
            },
            body: JSON.stringify({
                token: this.token,
                action: action,
                actionParam: actionParam
            })
        }).then(r => r.json());
        return res;
    }

    async slotClick(square, index) {


    }

    async initNewBoard() {
        
        this.gameboard = new GameBoard(27);
        $d("#game_board_container").innerHTML = '';
        $d("#game_board_container").appendChild(this.gameboard.html);

        this.gameboard.setBoardState(this.boardState.slotStates);
        this.gameboard.slots.forEach((slot, index) => {
            slot.click(e => this.slotClick(slot, index));
        });
    }

    async terminateGame() {
        await fetch("./Game/TerminateGame", {
            method: 'POST',
            headers: {
                'Accept': 'application/json',
                'Content-Type': 'application/json'
            },
            body: JSON.stringify({
                SessionId: this.sessionId,
            })
        }).catch(e =>{});
        Game.delete_cookie('mg_token');
    }

    async removeOldBoard() {
        if (this.gameboard !== undefined) {
            this.gameboard.hide(true);
            await Game.delay(1500);
            $('.game_board_container').removeChild(this.gameboard.html);
        }
    }

    static delay(t) {
        return new Promise(res => setTimeout(res, t));
    }

    updateGameinfo(gameinfo) {
        this.sessionId = gameinfo.sessionId;
        this.state = gameinfo.state;
        this.boardState = gameinfo.boardState,
        this.aiTimeSec = gameinfo.aiTimeSec;
        this.moveLeft = gameinfo.moveLeft;
        this.initTime = gameinfo.initTime;
        this.lastUpdatedTime = gameinfo.lastUpdatedTime;
        this.updateDisplayInfo();
    }

    updateDisplayInfo() {
        $d('#white_score').innerText = this.boardState.blackScore;
        $d('#black_score').innerText = this.boardState.whiteScore;
        $d('#move_left').innerText = this.moveLeft;
    }

    static translateSize(size) {
        let x = Math.floor(Math.sqrt(size));
        let y = x ** 2 == size ? x : x + 1;
        return {
            x: x,
            y: y
        };
    }

}

Game.gameMain().then(game => {
    console.log('game init success');
});

