import { Button, Box, Typography } from '@mui/material';
import React, { useEffect, useState } from 'react';
import { RTableCell as Cell, RTableRow as Row } from './ResultsTable';
import { ErasBold } from '../Eras';
import OutputDialog from './OutputDialog';

const EulerianPath = ({ data }) => {
    const [open, setOpen] = useState(false);
    useEffect(() => {
        console.log(data);
    }, [data]);

    const handleClick = () => {
        setOpen(!open);
    }

    const loadItems = (ps, records) => {
        return ps.slice(0, records).map((p, index) => (
            <Row key={index}>
                <Cell>{p.from}</Cell>
                <Cell>{p.to}</Cell>
            </Row>
        ));
    }

    return (<>
        <ErasBold fontSize={20} mb={1}>Eulerian Path</ErasBold>
        <Box sx={{ display: 'flex', justifyContent: 'center', alignItems: 'center', gap: 10 }}>
            <Box>
                <Typography fontSize={15}>Start: [{data.start}]</Typography>
                <Typography fontSize={15}>End: [{data.end}]</Typography>
            </Box>
            <Button variant='contained' color='info' onClick={handleClick}>Details</Button>
            <OutputDialog
                title='Eulerian Path Details'
                columns={['From', 'To']}
                open={open}
                handleClick={handleClick}
                dataArray={data.path}
                loadItems={loadItems}
            />
        </Box>
    </>);
}

export default EulerianPath;