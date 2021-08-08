$a = document.querySelectorAll.bind(document);
$d = document.querySelector.bind(document);
$c = document.createElement.bind(document);

const ROW_NUMBER_MAP = [5, 6, 7, 8, 9, 8, 7, 6, 5];

const MOVE_TABLE =
    [
        //0 1ST ROW
        [-1, -1, -1, 1, 6, 5],
        //1
        [0, -1, -1, 2, 7, 6],
        //2
        [1, -1, -1, 3, 8, 7],
        //3
        [2, -1, -1, 4, 9, 8],
        //4
        [3, -1, -1, -1, 10, 9],
        //5 2ND ROW
        [-1, -1, 0, 6, 12, 11],
        //6
        [5, 0, 1, 7, 13, 12],
        //7
        [6, 1, 2, 8, 14, 13],
        //8
        [7, 2, 3, 9, 15, 14],
        //9
        [8, 3, 4, 10, 16, 15],
        //10
        [9, 4, -1, -1, 17, 16],
        //11 3RD ROW
        [-1, -1, 5, 12, 19, 18],
        //12
        [11, 5, 6, 13, 20, 19],
        //13
        [12, 6, 7, 14, 21, 20],
        //14
        [13, 7, 8, 15, 22, 21],
        //15
        [14, 8, 9, 16, 23, 22],
        //16
        [15, 9, 10, 17, 24, 23],
        //17
        [16, 10, -1, -1, 25, 24],
        //18 4TH ROW
        [-1, -1, 11, 19, 27, 26],
        //19
        [18, 11, 12, 20, 28, 27],
        //20
        [19, 12, 13, 21, 29, 28],
        //21
        [20, 13, 14, 22, 30, 29],
        //22
        [21, 14, 15, 23, 31, 30],
        //23
        [22, 15, 16, 24, 32, 31],
        //24
        [23, 16, 17, 25, 33, 32],
        //25
        [24, 17, -1, -1, 34, 33],
        //26 5TH ROW
        [-1, -1, 18, 27, 35, -1],
        //27
        [26, 18, 19, 28, 36, 35],
        //28
        [27, 19, 20, 29, 37, 36],
        //29
        [28, 20, 21, 30, 38, 37],
        //30
        [29, 21, 22, 31, 39, 38],
        //31
        [30, 22, 23, 32, 40, 39],
        //32
        [31, 23, 24, 33, 41, 40],
        //33
        [32, 24, 25, 34, 42, 41],
        //34
        [33, 25, -1, -1, -1, 42],
        //35 6TH ROW
        [-1, 26, 27, 36, 43, -1],
        //36
        [35, 27, 28, 37, 44, 43],
        //37
        [36, 28, 29, 38, 45, 44],
        //38
        [37, 29, 30, 39, 46, 45],
        //39
        [38, 30, 31, 40, 47, 46],
        //40 
        [39, 31, 32, 41, 48, 47],
        //41
        [40, 32, 33, 42, 49, 48],
        //42
        [41, 33, 34, -1, -1, 49],
        //43 7TH ROW
        [-1, 35, 36, 44, 50, -1],
        //44
        [43, 36, 37, 45, 51, 50],
        //45
        [44, 37, 38, 46, 52, 51],
        //46
        [45, 38, 39, 47, 53, 52],
        //47
        [46, 39, 40, 48, 54, 53],
        //48
        [47, 40, 41, 49, 55, 54],
        //49
        [48, 41, 42, -1, -1, 55],
        //50 8TH ROW
        [-1, 43, 44, 51, 56, -1],
        //51
        [50, 44, 45, 52, 57, 56],
        //52
        [51, 45, 46, 53, 58, 57],
        //53
        [52, 46, 47, 54, 59, 58],
        //54
        [53, 47, 48, 55, 60, 59],
        //55
        [54, 48, 49, -1, -1, 60],
        //56 9TH ROW
        [-1, 50, 51, 57, -1, -1],
        //57
        [56, 51, 52, 58, -1, -1],
        //58
        [57, 52, 53, 59, -1, -1],
        //59
        [58, 53, 54, 60, -1, -1],
        //60
        [59, 54, 55, -1, -1, -1]
    ];

class Slot {
    constructor(index, size = 10) {
        this.index = index;
        this.state = 0;
        this.cssState = 'empty';
        this.isSelected = false;
        this.isIndicated = false;
        this.html = document.createElement('div');
        this.html.classList.add(this.cssState);
        this.html.style.height = size + 'vw';
        this.html.style.width = size + 'vw';
        this.html.classList.add('slot');
        let marble = document.createElement('div');
        marble.classList.add('marble');
        this.marble = marble;
        this.html.appendChild(marble);
    }

    setState() {
        const map = {
            0: 'empty',
            1: 'black',
            2: 'white'
        }

        this.html.classList.remove(this.cssState);

        if (arguments.length > 0 && arguments[0] !== undefined) {
            this.state = arguments[0];
        } else {
            this.state = 0;
        }
        this.cssState = map[this.state];
        this.html.classList.add(this.cssState);
    }

    select(isSelected) {
        this.isSelected = isSelected;
        if (isSelected) {
            this.html.classList.add('selected');
        } else {
            this.html.classList.remove('selected');
        }
    }

    indicate(isIndicated) {
        this.isIndicated = isIndicated;
        if (isIndicated) {
            this.html.classList.add('indicated');
        } else {
            this.html.classList.remove('indicated');
        }
    }

    click(fn) {
        this.html.addEventListener('click', e => fn(e));
    }

    hover(fn) {
        this.html.addEventListener('mouseover', e => fn(e));
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

    unSelectAll() {
        this.slots.forEach(s => s.select(false));
    }

    indicateSlots(list) {
        this.slots.forEach(s => s.indicate(false));
        list.forEach(i => this.slots[i].indicate(true));
    }

    unIndicateAll() {
        this.slots.forEach(s => s.indicate(false));
    }

    setBoardState(slotStates) {
        let i = 0
        slotStates.forEach(state => {
            this.slots[i++].setState(state);
        });
    }
}

class Game {
    constructor() { }

    static game = null;

    static createCookie(name, value, minute) {
        let expires = null;
        if (minute) {
            let date = new Date();
            date.setTime(date.getTime() + (minute * 60 * 1000));
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
        let token = Game.getCookie('mg_token');

        if (token === "") {
            Game.game = await Game.newGame();
        } else {
            Game.game = await Game.resumeGame(token);
        }
        $d('#terminate_btn').addEventListener('click', async e => {
            if (confirm("Are you sure you want to terminate?")) {
                await Game.game.terminateGame();
                Game.game = await Game.newGame();
            }
        });
    }

    static async newGame() {
        $("#create_game_modal").modal({
            backdrop: 'static',
            keyboard: false
        });

        $d("#ai_time").addEventListener('change', e => {
            let val = e.target.value;
            if (!Number.isInteger(val)) {
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
            if (!Number.isInteger(val)) {
                e.target.value = Math.round(val);
                val = e.target.value;
            }
            if (val < 10) {
                e.target.value = 10;
            }
        });

        let game = new Game();

        await new Promise(res =>
            $d('#start_btn').addEventListener('click', async e => {
                let aiTime = $d("#ai_time").value;
                let maxMove = $d("#max_move").value;
                let initState = 0;
                for (const ele of $a("input[name='init_state_radio']")) {
                    if (ele.checked) {
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
        Game.createCookie("mg_token", gameinfo.sessionId, 30);
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

    async gameSendMove(validMove) {
        return await fetch("./Game/Move", {
            method: 'POST',
            headers: {
                'Accept': 'application/json',
                'Content-Type': 'application/json'
            },
            body: JSON.stringify({
                SessionId: this.sessionId,
                Move: validMove
            })
        }).then(r => r.json());
    }

    aiLoading(isLoading){
        if(isLoading){
            document.body.classList.add("loading");
            $('#ai_search_text').show();
        } else{
            document.body.classList.remove("loading");
            $('#ai_search_text').hide();
        }
    }

    async updateGameBeforeMove(move){
        this.state = 2;
        this.boardState = move.state;
        this.isBlackTurn = !this.isBlackTurn;
        this.validMoves = [];
        this.gameboard.setBoardState(this.boardState.slotStates);
        this.selected = [];
        this.gameboard.unSelectAll();
        this.gameboard.unIndicateAll();
        this.aiLoading(true);
        //pop ai thinking
    }

    async updateGameAfterMove(move){
        let gameinfo = await this.gameSendMove(move);
        this.updateGameinfo(gameinfo);
        this.gameboard.setBoardState(this.boardState.slotStates);
        this.selected = [];
        this.aiLoading(false);
    }

    async slotClick(slot, index) {
        if(this.state != 1){
            return;
        }
        let ownColorState = this.isBlackTurn ? 1 : 2;
        if (slot.state === ownColorState) {
            this.trySelect(slot, index);
        } else {
            this.tryMove(slot, index);
        }
    }

    async tryMove(slot, index) {
        if (this.selected.length === 0) {
            return;
        }
        let ownColorState = this.isBlackTurn ? 1 : 2;
        if (slot.state == ownColorState) {
            return;
        }
        for (const i of Array(6).keys()) {
            for(const s of this.selected){
                if (MOVE_TABLE[s][i] == index) {
                    let key = this.selected.join(',');
                    if ((key in this.validMoveMap) && (i in this.validMoveMap[key])) {
                        let move = this.validMoveMap[key][i];
                        this.updateGameBeforeMove(move);
                        await this.updateGameAfterMove(move);
                        return;
                    }
                }
            }
        }
    }

    async trySelect(slot, index) {
        this.gameboard.unIndicateAll();
        if (slot.isSelected) {
            const index = this.selected.indexOf(slot);
            this.selected.splice(index, 1);
        } else {
            let success = false;
            switch (this.selected.length) {
                case 0:
                    this.selected.push(index);
                    success = true;
                    break;
                case 1:
                    for (const i of Array(6).keys()) {
                        if (MOVE_TABLE[index][i] == this.selected[0]) {
                            this.selected.push(index);
                            success = true;
                            break;
                        }
                    }
                    if (!success) {
                        this.selected[0] = index;
                        success = true;
                    }
                    break;
                case 2:
                    for (const i of Array(6).keys()) {
                        if (
                            (MOVE_TABLE[index][i] == this.selected[0] && MOVE_TABLE[this.selected[0]][i] == this.selected[1])
                            || (MOVE_TABLE[index][i] == this.selected[1] && MOVE_TABLE[this.selected[1]][i] == this.selected[0])
                        ) {
                            this.selected.push(index);
                            success = true;
                            break;
                        }
                    }
                    if (!success) {
                        this.selected = [];
                        this.selected.push(index);
                        success = true;
                    }
                    break;
                default:
                    this.selected = [];
                    this.selected.push(index);
                    success = true;
            }
        }

        this.selected.sort();
        this.gameboard.selectSlots(this.selected);
    }

    async slotHover(slot, index) {
        if(this.state != 1){
            return;
        }
        if (this.selected.length === 0) {
            this.gameboard.unIndicateAll();
            return;
        }
        let ownColorState = this.isBlackTurn ? 1 : 2;
        if (slot.state == ownColorState) {
            this.gameboard.unIndicateAll();
            return;
        }
        for (const i of Array(6).keys()) {
            for(const s of this.selected){
                if (MOVE_TABLE[s][i] == index) {
                    let key = this.selected.join(',');
                    if ((key in this.validMoveMap) && (i in this.validMoveMap[key])) {
                        let move = this.validMoveMap[key][i];
                        let slots = move.action.movements.map(m => m.toIndex).filter(si => si != -1);
                        this.gameboard.indicateSlots(slots)
                        return;
                    }
                }
            }
        }
    }

    async initNewBoard() {

        this.gameboard = new GameBoard(27);
        $d("#game_board_container").innerHTML = '';
        $d("#game_board_container").appendChild(this.gameboard.html);

        this.gameboard.setBoardState(this.boardState.slotStates);
        this.selected = [];
        this.gameboard.slots.forEach((slot, index) => {
            slot.click(e => this.slotClick(slot, index));
            slot.hover(e => this.slotHover(slot, index));
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
        }).catch(e => { });
        Game.delete_cookie('mg_token');
    }

    static delay(t) {
        return new Promise(res => setTimeout(res, t));
    }

    updateGameinfo(gameinfo) {
        this.sessionId = gameinfo.sessionId;
        this.state = gameinfo.state;
        this.boardState = gameinfo.boardState;
        this.isBlackTurn = gameinfo.isBlackTurn;
        this.validMoves = gameinfo.validMoves;
        this.aiTimeSec = gameinfo.aiTimeSec;
        this.moveLeft = gameinfo.moveLeft;
        this.initTime = gameinfo.initTime;
        this.lastUpdatedTime = gameinfo.lastUpdatedTime;
        this.buildValidMoveMap();
        this.updateDisplayInfo();
    }

    buildValidMoveMap() {
        this.validMoveMap = {};

        for (const actState of this.validMoves) {
            let moveSlots = []
            let ownColorState = this.isBlackTurn ? 1 : 2;
       
            for (const move of actState.action.movements) {
                if (this.boardState.slotStates[move.fromIndex] == ownColorState) {
                    moveSlots.push(move.fromIndex);
                }
            }
            let key = moveSlots.sort().join(',');
            if (!(key in this.validMoveMap)) {
                this.validMoveMap[key] = {};
            }
            this.validMoveMap[key][actState.action.direction] = actState;
        }

    }

    updateDisplayInfo() {
        $d('#white_score').innerText = this.boardState.whiteScore;
        $d('#black_score').innerText = this.boardState.blackScore;
        $d('#move_left').innerText = this.moveLeft;

        const stateMap = {
            8: "Black Winned",
            16: "White Winned",
            32: "Draw",
        };

        if (this.state >= 8) {
            $d('#game_result_text').innerText = stateMap[this.state];

            $('#game_finish_modal').on('hidden.bs.modal', async e => {
                Game.game = await Game.newGame();
            });

            $('game_finish_modal_close_btn').on('click', async e=>{
                $('#game_finish_modal').modal('hide');
            })

            $('#game_finish_modal').modal({
                backdrop: 'static',
                keyboard: false
            });
        }
    }
}

Game.gameMain().then(() => {
    console.log('game init success');
});

