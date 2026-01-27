import clsx from 'clsx';
import Heading from '@theme/Heading';
import styles from './styles.module.css';

const FeatureList = [
  {
    title: 'Coupled Multiphysics',
    description: (
      <>
        Solves coupled electrochemical reactions, multi-species transport,
        and fluid flow with Darcy-Forchheimer drag in porous electrode media.
      </>
    ),
  },
  {
    title: 'Open-Source & Extensible',
    description: (
      <>
        Built on OpenFOAM with a modular code structure for easy maintenance
        and extensibility. Freely available for collaborative research.
      </>
    ),
  },
  {
    title: 'Validated & Verified',
    description: (
      <>
        Verified against COMSOL Multiphysics and validated with experimental
        data from all-iron RFB cells with NIPS electrodes.
      </>
    ),
  },
];

function Feature({title, description}) {
  return (
    <div className={clsx('col col--4')}>
      <div className="text--center padding-horiz--md">
        <Heading as="h3">{title}</Heading>
        <p>{description}</p>
      </div>
    </div>
  );
}

export default function HomepageFeatures() {
  return (
    <section className={styles.features}>
      <div className="container">
        <div className="row">
          {FeatureList.map((props, idx) => (
            <Feature key={idx} {...props} />
          ))}
        </div>
      </div>
    </section>
  );
}
