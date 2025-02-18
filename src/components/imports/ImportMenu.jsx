import { Box, Divider, ListItemIcon, ListItemText, Menu, MenuItem, MenuList } from "@mui/material";
import BackupTableIcon from '@mui/icons-material/BackupTable';
import DataObjectIcon from '@mui/icons-material/DataObject';
import CodeIcon from '@mui/icons-material/Code';
import DataArrayIcon from '@mui/icons-material/DataArray';
import AutoModeIcon from '@mui/icons-material/AutoMode';
import React, { useEffect, useState } from "react";
import ImportCSV from "./ImportCSV";
import ImportJSON from "./ImportJSON";
import ImportGML from "./ImportGML";
import ImportGEXF from "./ImportGEXF";
import ImportAutoRandom from "./ImportAutoRandom";

const ImportMenu = ({ anchorEl, setAnchorEl, module, updateGraph, setLoading }) => {
    const [openDialog, setOpenDialog] = useState({
        csv: false,
        json: false,
        gexf: false,
        gml: false,
        auto: false
    });
    const [active, setActive] = useState('');

    const handleOpenDialog = (type) => {
        setOpenDialog({ ...openDialog, [type]: true });
        setActive(type)
        setAnchorEl(null)
    };
    
    const handleCloseDialog = (type) => {
        setOpenDialog({ ...openDialog, [type]: false });
        setActive('')
    };

    const handleGraphUpdate = (nodes, edges, directed) => {
        console.log(nodes)
        console.log(edges)
        updateGraph(nodes, edges, directed);
        handleCloseDialog(active);
    }

    return(<>
        <Menu
            anchorEl={anchorEl}
            keepMounted
            open={Boolean(anchorEl)}
            onClose={() => setAnchorEl(null)}
            sx={{ width: 320, maxWidth: '100%' }}
        >
            <Box sx={{ width: 320, maxWidth: '100%' }} elevation={10}>
                <MenuList sx={{ padding: 0 }}>
                    <MenuItem onClick={() => handleOpenDialog('csv')}>
                        <ListItemIcon><BackupTableIcon /></ListItemIcon>
                        <ListItemText>Import CSV</ListItemText>
                    </MenuItem>

                    <MenuItem onClick={() => handleOpenDialog('json')}>
                        <ListItemIcon><DataObjectIcon /></ListItemIcon>
                        <ListItemText>Import JSON</ListItemText>
                    </MenuItem>

                    <MenuItem onClick={() => handleOpenDialog('gexf')}>
                        <ListItemIcon><CodeIcon /></ListItemIcon>
                        <ListItemText>Import GEXF</ListItemText>
                    </MenuItem>

                    <MenuItem onClick={() => handleOpenDialog('gml')}>
                        <ListItemIcon><DataArrayIcon /></ListItemIcon>
                        <ListItemText>Import GML</ListItemText>
                    </MenuItem>

                    <Divider />

                    <MenuItem onClick={() => handleOpenDialog('auto')}>
                        <ListItemIcon><AutoModeIcon /></ListItemIcon>
                        <ListItemText>Generate Graph</ListItemText>
                    </MenuItem>
                </MenuList>
            </Box>
        </Menu>
        <ImportCSV
            open={openDialog.csv}
            onClose={() => handleCloseDialog('csv')}
            module={module}
            updateGraph={handleGraphUpdate}
            setLoading={setLoading}
        />
        <ImportJSON
            open={openDialog.json}
            onClose={() => handleCloseDialog('json')}
            module={module}
            updateGraph={handleGraphUpdate}
            setLoading={setLoading}
        />
        <ImportGML
            open={openDialog.gml}
            onClose={() => handleCloseDialog('gml')}
            module={module}
            updateGraph={handleGraphUpdate}
            setLoading={setLoading}
        />
        <ImportGEXF
            open={openDialog.gexf}
            onClose={() => handleCloseDialog('gexf')}
            module={module}
            updateGraph={handleGraphUpdate}
            setLoading={setLoading}
        />
        <ImportAutoRandom
            open={openDialog.auto}
            onClose={() => handleCloseDialog('auto')}
            module={module}
            updateGraph={handleGraphUpdate}
            setLoading={setLoading}
        />
    </>)
}

export default ImportMenu;