import { FC } from 'react';
import { Navigate, Routes, Route } from 'react-router-dom';

import DemoProject from './DemoProject';

const ProjectRouting: FC = () => {
  return (
    <Routes>
      {
        // Add the default route for your project below
      }
      <Route path="/*" element={<Navigate to="settings/information" />} />
      {
        // Add your project page routes below.
      }
      <Route path="settings/*" element={<DemoProject />} />
    </Routes>
  );
};

export default ProjectRouting;
