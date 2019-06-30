type item = {
    id: int,
    title: string,
    completed: bool
};

let str = ReasonReact.string;
/** Nested todo item component */
module TodoItem = {
    [@react.component]
    let make = (~item, ~onToggle) => {
        <div className="item" onClick=((_event) => onToggle())>
        <input type_="checkbox"
        checked=(item.completed)/>
        (str(item.title))
        </div>
    }
}

let valueFromEvent = (_event): string => _event -> ReactEvent.Form.target##value;
/** _event |> ReactEvent.Form.target##value;or _event |> ReactEvent.Form.target |> (x=> x##value) */
/** Another nested component: Input */
module Input = {
    type state = string;
    [@react.component]
    let make = (~onSubmit) => {
    let (text, setText) = React.useReducer((oldText, newText) => newText, "");
    <input
        value=text
        type_="text"
        placeholder="Write something to do"
        onChange=((event) => setText(valueFromEvent(event)))
        onKeyDown=((_event)=>
            if (ReactEvent.Keyboard.key(_event) == "Enter"){
                onSubmit(text);
                setText("")
            }
        )
    />
    };
};

/** Main Todo Component */
type state = {
    items: list(item)
};

type action = 
  | AddItem(string)
  | ToggleItem(int);

let lastId = ref(0); /** a mutable let */
let newItem = (text) => {
    lastId := lastId^ + 1;
    {id: lastId^,
    title: text, 
    completed: false
    }
};

let itemCount(numItems) = if (numItems==1) {
    (string_of_int(numItems)++ " item")
}else {
    (string_of_int(numItems)++ " items")
};

/** let count = (~completed, items) => {
    switch items {
    | [] => 0
    | [a] => 1
    | [a, ...rest] => {
        let total = ref(0);
        for(index in 0 to List.length(items)){
            if(Array.of_list(items)[index].completed == completed){
                total := total^ + 1;
            };
        };
        total.contents
    };
    }
}; 


let finished = (items, ~completed) => {
    string_of_int(count(items, ~completed)) ++
    " Finished Tasks"
};

let notFinished = (items, ~completed) => {
    string_of_int(count(items, ~completed)) ++
    " Not Finished Tasks"
}; */

[@react.component]
let make = () => {
    let ({items}, dispatch) = React.useReducer((state, action) => {
        switch action {
        | AddItem(text) => {items: [newItem(text), ...state.items]}
        | ToggleItem(id) => 
            let items = List.map(
                (item) => item.id === id ? {...item, completed: !item.completed} : item, 
            state.items
        );
        {items: items}
        }
    }, {
        items: [
            {id: 0,
            title: "Write some things to do", 
            completed: false}
        ]
    });
    let numItems = List.length(items);
    let footer = itemCount(numItems);
    <div className="app">
        <div className="title">
           (str("Todo List"))
        </div>
        <div className="input">
        <Input onSubmit=((text) => dispatch(AddItem(text))) />
        </div>
        <div className="items">
           (
                List.map((item) => <TodoItem
                key=(string_of_int(item.id))
                onToggle= (() => dispatch(ToggleItem(item.id)))
                item />, items) |> Array.of_list |> React.array
           )
        </div>
        <div className="allCount">
            (str(footer))
        </div>
        /** <div className="finishedCount">
            (str(finished(items, ~completed=true)))
        </div>
        <div className="notFinishedCount">
            (str(notFinished(items, ~completed=false)))
        </div>*/
   </div>
};