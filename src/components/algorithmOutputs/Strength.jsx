import { Button, Dialog, DialogActions, DialogContent, DialogTitle, Table, TableBody, TableContainer, TableHead, Paper, Box, Typography } from '@mui/material';
import React, { useEffect, useState } from 'react';
import { RTableCell, RTableRow } from './ResultsTable';
import { ErasBold, ErasMedium } from '../Eras';

const Strength = ({ data }) => {
    const [open, setOpen] = useState(false);
    const [centralities, setCentralities] = useState([]);
    useEffect(() => {
        console.log(data);
        data && setCentralities([...data.centralities].sort((a, b) => b.centrality - a.centrality));
    }, [data]);

    const handleClick = () => {
        setOpen(!open);
    }

    return (<>
        <ErasBold fontSize={20} mb={1}>Strength Centrality</ErasBold>
        <Box sx={{ display: 'flex', justifyContent: 'center', alignItems: 'center', gap: 10 }}>
            <Box>
                <Typography fontSize={15}>Maximum Centrality Node:</Typography>
                <Typography fontSize={15}>[{centralities[0] && centralities[0].node}]</Typography>
            </Box>
            <Button variant='contained' color='info' onClick={handleClick}>More Details</Button>
            <Dialog open={open} onClose={handleClick} fullWidth>
                <DialogTitle>
                    <ErasMedium>Node Strength Details</ErasMedium>
                </DialogTitle>
                <DialogContent>
                    <Typography variant='body2' pb={1}>Node strength will be exactly the same as degree centrality in an unweighted graph. Instead of taking the degree (number of connecting edges) of a node, the strength takes the sum of all the weights of connecting edges.</Typography>
                    <TableContainer component={Paper} style={{ maxWidth: 'auto', margin: 'auto' }}>
                        <Table size='small' sx={{ tableLayout: 'auto' }}>
                            <TableHead>
                                <RTableRow>
                                    <RTableCell>ID</RTableCell>
                                    <RTableCell>Node</RTableCell>
                                    <RTableCell>Centrality</RTableCell>
                                </RTableRow>
                            </TableHead>
                            <TableBody>
                                {centralities.map((c, index) => (
                                    <RTableRow key={index}>
                                        <RTableCell>{c.id}</RTableCell>
                                        <RTableCell>{c.node}</RTableCell>
                                        <RTableCell>{c.centrality}</RTableCell>
                                    </RTableRow>
                                ))}
                            </TableBody>
                        </Table>
                    </TableContainer>
                </DialogContent>
                <DialogActions>
                    <Button onClick={handleClick}>Close</Button>
                </DialogActions>
            </Dialog>
        
        </Box>
    </>);
}

export default Strength;