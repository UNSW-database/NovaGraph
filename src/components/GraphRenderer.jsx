import { Cosmograph, CosmographProvider } from '@cosmograph/react'
import React, { useRef, useCallback, useEffect, useState, useLayoutEffect } from 'react';
import chroma from "chroma-js";
import { Box, Button, IconButton, Tooltip } from '@mui/material';
import SettingsIcon from '@mui/icons-material/Settings';
import PauseIcon from '@mui/icons-material/Pause';
import PlayArrowIcon from '@mui/icons-material/PlayArrow';

export function GraphRenderer({ colors, nodes, links, colorAll }) {
    const cosmograph = useRef()
    const scale = chroma.scale(['#F7EBFF', '#6750C6']);
    /* Palette: https://mycolor.space/?hex=%236750C6&sub=1 (Spot Palette)
        - Dark: #6750c6
        - Default: #9f8fc3
        - Light: #f7ebff
        - Contrast (Green): #67baa7
    */

    const zoomToNode = useCallback((node, i, pos, event) => {
        if (node && i != undefined) {
            cosmograph.current?.selectNode(node);
            cosmograph.current?.zoomToNode(node);
        } else {
            cosmograph.current?.unselectNodes();
            cosmograph.current?.fitView(1000);
        }
    })

    // colorspace with 6B0072
    const getColor = (freq, id) => {
        if (colorAll && freq > 0) {
            return scale(freq / nodes.length).hex()
        } else if (freq > 0) {
            return scale(1).hex()
        } else if (colorAll) {
            return '#F05480'
        } else {
            return '#9f8fc3'
        }
    }

    return(
    <Box sx={{ display:'flex', flexDirection: 'column', height: '500px' }}>
        
        <CosmographProvider nodes={nodes} links={links}>
            <Cosmograph
                ref={cosmograph}
                //initialZoomLevel={1}
                disableSimulation={false}
                //backgroundColor='#151515'
                nodeSize={20}
                nodeColor={(_node, id) => getColor(colors[id], id)}
                linkColor={(link) => colors[`${link.source}-${link.target}`] > 0 ? '#67baa7' : null}
                nodeGreyoutOpacity={0.1}
                linkWidth={(link) => colors[`${link.source}-${link.target}`] > 0 ? 3 : 0.1}

                linkArrows={false}
                
                //renderHoveredNodeRing={true}
                //hoveredNodeRingColor='#4B5BBF'
                linkGreyoutOpacity={0}
                //simulationLinkDistance={20}
                //simulationLinkSpring={1.5}
                simulationRepulsion={2}
                //simulationGravity={1}
                simulationLinkSpring={0.01}
                simulationDecay={100000}
                onClick={zoomToNode}
            />

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

        <Box display='flex'>
            <Tooltip title="Play Simulation">
                <IconButton aria-label='play-simulation' onClick={() => cosmograph.current?.start()}>
                    <PlayArrowIcon />
                </IconButton>
            </Tooltip>
            <Tooltip title="Pause Simulation">
                <IconButton aria-label='pause-simulation' onClick={() => cosmograph.current?.pause()}>
                    <PauseIcon />
                </IconButton>
            </Tooltip>
            
            

            <Box flexGrow={1} />
            
            <Tooltip title="Graph Options">
                <IconButton aria-label='graph-options'>
                    <SettingsIcon />
                </IconButton>
            </Tooltip>
        </Box>
        
    </Box>
    )
}