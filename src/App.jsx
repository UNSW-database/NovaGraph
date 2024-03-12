import { useState, useEffect } from 'react'
import createModule from './graph'
import './App.css'
import { CosmographProvider } from '@cosmograph/react'
import { GraphRenderer } from './components/GraphRenderer';
import { Box, Button, ButtonGroup, Typography } from '@mui/material';
import { Accordion, AccordionDetails, AccordionSummary } from './components/Accordion';

import { ThemeProvider, createTheme } from '@mui/material/styles';
import CssBaseline from '@mui/material/CssBaseline';

const darkTheme = createTheme({
  palette: {
    mode: 'dark',
    primary: {
      main: '#FDF7FF'
    }
  },
});



function App() {
  const [wasmModule, setWasmModule] = useState();
  const [expanded, setExpanded] = useState('panel1');

  const [nodes, setNodes] = useState([]);
  const [edges, setEdges] = useState([]);
  const [colorMap, setColorMap] = useState({});
  const [red, setRed] = useState(false);
  const [text, setText] = useState("");

  useEffect(() => {
    createModule().then(mod => {
      setWasmModule(mod)
      mod.initGraph(); // initialize the graph in C++
      const graph = mod.getGraph(); // get the graph as a JS array

      let nodesTmp = []
      let edgesTmp = []

      for (var i = 0; i < graph.length; i++) {
          nodesTmp.push({ id: i }) // create a node for each index
          for (var j = 0; j < graph[i].length; j++) {
              if (graph[i][j] === 1) { // if there is an edge
                  edgesTmp.push({
                      source: i,
                      target: j
                  })
              }
          }
      }

      setNodes(nodesTmp)
      setEdges(edgesTmp)

      window.onunload = () => {
        mod.cleanupGraph()
      }
    })
  }, []);

  useEffect(() => {
    console.log(colorMap)
  }, [colorMap])

  const handleAccordianChange = (panel) => (event, newExpanded) => {
    setExpanded(newExpanded ? panel : false);
  };

  const doAreConnected = () => {
    const source = prompt("Enter source vertex", "0");
    const target = prompt("Enter target vertex", "0");
    let test = wasmModule.vertices_are_connected(2,9)
    console.log(test)
  }

  const doDijkstraSingle = () => {
    const source = prompt("Enter source vertex", "0");
    const target = prompt("Enter target vertex", "0");
    const response = wasmModule.dijkstra_source_to_target(parseInt(source), parseInt(target));
    setColorMap(response.colorMap)
    setText(response.message)
    setRed(false)
  }
  const doDijkstraMulti = () => {
    const source = prompt("Enter source vertex", "0");
    const response = wasmModule.dijkstra_source_to_all(parseInt(source));
    setColorMap(response.colorMap)
    setText(response.message)
    setRed(true)
  }
  const doYen = () => {
    const source = prompt("Enter source vertex", "0");
    const target = prompt("Enter target vertex", "0");
    const kInput = prompt("Enter k (number of paths)");
    const k = parseInt(kInput) || 1;
    const response = wasmModule.yens_algorithm(parseInt(source), parseInt(target), k);
    setColorMap(response.colorMap)
    setText(response.message)
    setRed(true)
  }
  const doBFSingle = () => {
    const source = prompt("Enter source vertex", "0");
    const target = prompt("Enter target vertex", "0");
    const response = wasmModule.bellman_ford_source_to_target(parseInt(source), parseInt(target));
    setColorMap(response.colorMap)
    setText(response.message)
    setRed(false)
  }
  const doBFMulti = () => {
    const source = prompt("Enter source vertex", "0");
    const response = wasmModule.bellman_ford_source_to_all(parseInt(source));
    setColorMap(response.colorMap)
    setText(response.message)
    setRed(true)
  }
  const doBFS = () => {
    const source = prompt("Enter source vertex", "0");
    const response = wasmModule.bfs(parseInt(source));
    setColorMap(response.colorMap)
    setText(response.message)
    setRed(true)
  }
  const doDFS = () => {
    const source = prompt("Enter source vertex", "0");
    const response = wasmModule.dfs(parseInt(source));
    setColorMap(response.colorMap)
    setText(response.message)
    setRed(true)
  }
  const doRW = () => {
    const start = prompt("Enter starting vertex", "0");
    const steps = prompt("Enter step count", "0");
    const response = wasmModule.random_walk(parseInt(start), parseInt(steps));
    setColorMap(response.colorMap)
    setText(response.message)
    setRed(true)
  }
  const doMST = () => {
    const response = wasmModule.min_spanning_tree();
    setColorMap(response.colorMap)
    setText(response.message)
    setRed(true)
  }

  return (
    <ThemeProvider theme={darkTheme}>
      {/*<CssBaseline /> changes backgroundColor to black */}
      <h1>NovaGraph</h1>
      
      <Box sx={{ display: 'flex', gap: 1 }}>
        <CosmographProvider nodes={nodes} links={edges} id="hicosmos">
          <GraphRenderer colors={colorMap} nodes={nodes} colorAll={red} />
          {/*
          TODO: have some sort of a form/modal here:

          Select source vertex: (info hover icon)
          <CosmographSearch onSelectResult={event => setState(event.id)} />

          Select target vertex: (info hover icon)
          <CosmographSearch onSelectResult={event => console.log(event)} />

          Enter some other variable: _______

          [Cancel] [Go]
        */}

        </CosmographProvider>
        

        <Box>
          <Accordion expanded={expanded === 'panel1'} onChange={handleAccordianChange('panel1')}>
            <AccordionSummary aria-controls="panel1-content" id="panel1-header">
              <Typography>Path Finding & Search Algorithms</Typography>
            </AccordionSummary>
            <AccordionDetails>
              <ButtonGroup orientation='vertical' variant='text'>
                <Button onClick={doAreConnected}>Connection Status</Button>
                <Button onClick={doDijkstraSingle}>Dijkstra &#40;A to B&#41;</Button>
                <Button onClick={doDijkstraMulti}>Dijkstra &#40;A to all&#41;</Button>
                <Button onClick={doYen}>Yen's Shortest &#40;A to B&#41;</Button>
                <Button onClick={doBFSingle}>Bellman-Ford &#40;A to B&#41;</Button>
                <Button onClick={doBFMulti}>Bellman-Ford &#40;A to all&#41;</Button>
                <Button onClick={doBFS}>BFS</Button>
                <Button onClick={doDFS}>DFS</Button>
                <Button onClick={doRW}>Random Walk</Button>
                <Button onClick={doMST}>Min Spanning Tree</Button>
              </ButtonGroup>
            </AccordionDetails>
          </Accordion>

          <Accordion expanded={expanded === 'panel2'} onChange={handleAccordianChange('panel2')}>
            <AccordionSummary aria-controls="panel2-content" id="panel2-header">
              <Typography>Accordion 2</Typography>
            </AccordionSummary>
            <AccordionDetails>
              b
            </AccordionDetails>
          </Accordion>

        </Box>

      </Box>
      
      <h2>Output</h2>
      <hr />
      <pre>{text}</pre>
    </ThemeProvider>
  )
}

export default App
