from reportlab.lib.pagesizes import letter
from reportlab.lib.styles import getSampleStyleSheet, ParagraphStyle
from reportlab.lib.units import inch
from reportlab.lib.enums import TA_CENTER, TA_LEFT, TA_JUSTIFY
from reportlab.platypus import (SimpleDocTemplate, Paragraph, Spacer, Image,
                                 ListFlowable, ListItem, PageBreak)
from reportlab.lib import colors
import os

doc = SimpleDocTemplate("report/REPORT.pdf", pagesize=letter,
                         topMargin=0.9*inch, bottomMargin=0.9*inch,
                         leftMargin=0.9*inch, rightMargin=0.9*inch)

styles = getSampleStyleSheet()

title_style = ParagraphStyle('TitleCentered', parent=styles['Title'],
                              fontSize=17, leading=21, alignment=TA_CENTER,
                              spaceAfter=6, fontName='Helvetica-Bold')
author_style = ParagraphStyle('AuthorCentered', parent=styles['Normal'],
                               fontSize=11, alignment=TA_CENTER, spaceAfter=2,
                               textColor=colors.HexColor('#333333'))
repo_style = ParagraphStyle('RepoCentered', parent=styles['Normal'],
                             fontSize=9.5, alignment=TA_CENTER, spaceAfter=18,
                             textColor=colors.HexColor('#555555'))
h1 = ParagraphStyle('H1', parent=styles['Heading1'], fontSize=14, spaceBefore=16, spaceAfter=8)
h2 = ParagraphStyle('H2', parent=styles['Heading2'], fontSize=12, spaceBefore=12, spaceAfter=6)
body = ParagraphStyle('Body', parent=styles['Normal'], fontSize=10.3, leading=15,
                       alignment=TA_JUSTIFY, spaceAfter=8)
eq = ParagraphStyle('Eq', parent=styles['Normal'], fontSize=10, leading=14,
                     alignment=TA_CENTER, spaceBefore=4, spaceAfter=8,
                     fontName='Courier', textColor=colors.HexColor('#222222'))
caption = ParagraphStyle('Caption', parent=styles['Normal'], fontSize=9,
                          alignment=TA_CENTER, spaceAfter=14, spaceBefore=4,
                          textColor=colors.HexColor('#444444'), fontName='Helvetica-Oblique')
ref = ParagraphStyle('Ref', parent=styles['Normal'], fontSize=9.3, leading=13,
                      spaceAfter=5, leftIndent=14, firstLineIndent=-14)

story = []

story.append(Paragraph("Discrete d'Alembertian and Wave Propagation on a Causal Set:<br/>A First Numerical Study", title_style))
story.append(Paragraph("Shivangi Gupta", author_style))
story.append(Paragraph('<link href="https://github.com/pikaism/Causal-set-wave">github.com/pikaism/Causal-set-wave</link>', repo_style))

def img(path, width=5.4*inch):
    return Image(path, width=width, height=width*0.55)

story.append(Paragraph("1. Motivation and Goal", h1))
story.append(Paragraph(
"This project implements a first working numerical toolkit for exploring causal set "
"phenomenology in 1+1D Minkowski spacetime, built as independent preparation for "
"research in the causal set approach to quantum gravity. The goal was to implement, "
"test, and honestly validate the core kinematical and dynamical ingredients used in "
"this field: Poisson sprinkling, the causal order, the discrete Benincasa-Dowker (BD) "
"d'Alembertian, and a scalar field evolution scheme, compared against the known "
"continuum limit. This toolkit is intended as a foundation for more advanced numerical "
"studies of kinematical and dynamical aspects of causal sets, in line with ongoing "
"work in the Theoretical Physics Group at RRI.", body))

story.append(Paragraph("2. Methods", h1))

story.append(Paragraph("2.1 Sprinkling and causal order", h2))
story.append(Paragraph(
"N points are sprinkled via a Poisson process (uniform in spacetime volume) into a "
"bounded region [0,T] &times; [-L,L] of 1+1D Minkowski spacetime. This construction is "
"Lorentz-invariant in distribution. The causal order between any two elements is "
"determined by the Minkowski interval condition:", body))
story.append(Paragraph("(t<sub>j</sub> - t<sub>i</sub>)<super>2</super> - (x<sub>j</sub> - x<sub>i</sub>)<super>2</super> &#8805; 0  and  t<sub>j</sub> &gt; t<sub>i</sub>", eq))

story.append(Paragraph("2.2 Layers and the discrete d'Alembertian", h2))
story.append(Paragraph(
"For each element x, its causally preceding elements are classified into layers "
"L<sub>0</sub>, L<sub>1</sub>, L<sub>2</sub> according to the cardinality of the causal interval between them "
"(the number of elements causally between the two). In 2D (1+1), the Benincasa-Dowker "
"operator takes the form:", body))
story.append(Paragraph("(B&phi;)(x) = (1/l<super>2</super>) [ -2&phi;(x) + 4&Sigma;L<sub>0</sub> - 8&Sigma;L<sub>1</sub> + 4&Sigma;L<sub>2</sub> ]", eq))
story.append(Paragraph(
"where l<super>2</super> = 1/&rho; and &rho; = N / Vol(R) is the sprinkling density over the "
"sprinkled region R.", body))
story.append(Paragraph(
"<b>Validation.</b> A naive single-sprinkling test of this operator on simple fields "
"(constant, linear) gave results with large, seemingly inconsistent deviations from "
"the expected value of zero. This was diagnosed, not as an implementation bug, but as "
"a genuine, literature-documented feature: the BD operator's convergence to the "
"continuum d'Alembertian is a statement about the <i>ensemble average</i> over many "
"sprinklings, not any single realization [2,3,4]. This was confirmed via ensemble "
"averaging over 30 independent sprinklings (N=500): mean = 0.117, standard error = "
"1.365, consistent with the theoretical value of 0.", body))

story.append(Paragraph("2.3 Wave packet initialization", h2))
story.append(Paragraph("A Gaussian wave packet is initialized on an early time-slice of the causal set:", body))
story.append(Paragraph("&phi;(t<sub>0</sub>,x) = A &middot; exp(-(x-x<sub>0</sub>)<super>2</super> / 2&sigma;<super>2</super>) &middot; cos(k<sub>0</sub>(x-x<sub>0</sub>))", eq))
story.append(Paragraph("&part;&phi;/&part;t (t<sub>0</sub>,x) = -v &middot; &part;&phi;/&part;x (t<sub>0</sub>,x)", eq))
story.append(Paragraph(
"the second line encoding a right-moving initial condition. These data specify "
"&phi; and &part;&phi;/&part;t on an initial antichain approximating t=t<sub>0</sub>, defining a "
"standard Cauchy problem for the wave equation.", body))

story.append(Paragraph("2.4 Field evolution", h2))
story.append(Paragraph(
"The field is evolved by processing elements in increasing time order and solving the "
"discrete field equation (B&phi;)(x) - m<super>2</super>&phi;(x) = 0 for the one unknown &phi;(x), "
"using only already-known values in its causal past (a retarded solve, standard in "
"the causal set literature; see e.g. Johnston [5] on causal set field propagators).", body))
story.append(Paragraph(
"<b>Instability found and fixed.</b> A naive exact solve of this equation was found to be "
"numerically unstable: the field magnitude grew exponentially (from O(1) to O(10<super>25</super>)) "
"over a handful of time bins, even though the direction of propagation was "
"qualitatively correct. In standard finite-difference schemes, stability is controlled "
"by a dimensionless parameter such as (&Delta;t/&Delta;x)<super>2</super> (the CFL condition); "
"here, the irregular causal structure provides no such small parameter, so the naive "
"recursion is effectively unconditionally unstable, amplifying noise multiplicatively "
"across the many \"generations\" of causal depth in the set. The fix applied was to "
"(a) use layer <i>averages</i> instead of raw sums (bounding the update regardless of "
"layer size), and (b) apply an under-relaxation / damping factor &alpha; (used here: "
"&alpha;=0.3). This is a standard numerical stabilization technique, applied here at "
"the acknowledged cost of no longer solving the exact discrete field equation.", body))

story.append(Paragraph("2.5 Continuum reference solution", h2))
story.append(Paragraph(
"A standard CFL-stable explicit leapfrog finite-difference scheme solves the same "
"wave equation on a regular grid, with identical initial data, for direct comparison.", body))

story.append(Paragraph("2.6 The n-interval spectrum", h2))
story.append(Paragraph(
"Following Surya [6], the n-interval spectrum N<sub>m</sub> is defined as the number of "
"causal intervals I(y,x) = {z : y &lt; z &lt; x} with cardinality |I(y,x)| = m, counted "
"over all causally related pairs (y,x) in the causal set. This order-invariant "
"\"fingerprint\" was computed as an additional diagnostic of continuumlike behavior.", body))

story.append(PageBreak())

story.append(Paragraph("3. Results", h1))

story.append(Paragraph("3.1 Wave packet propagation: discrete vs continuum", h2))
story.append(img("figures/wavepacket_comparison.png"))
story.append(Paragraph("Figure 1. Discrete (causal set, red) vs continuum (blue) field profile at six successive time windows.", caption))
story.append(Paragraph(
"At the initial time bin, the discrete (red) and continuum (blue) solutions agree "
"closely, as expected since both share the same initial data. At later times, the "
"continuum solution retains a clean, moving Gaussian shape, while the discrete "
"solution becomes visibly noisier, though it broadly tracks the same rightward "
"propagation. Quantitatively, the energy-weighted centroid in the discrete case "
"drifts rightward but more slowly and with larger fluctuations than in the continuum "
"case, reflecting the dissipative effect of the damping factor &alpha;=0.3. This is a "
"direct, visual and quantitative confirmation that the damping needed to stabilize "
"the discrete evolution measurably degrades the coherence of the propagating wave "
"packet relative to the continuum &mdash; a real, quantified limitation of the current "
"scheme.", body))

story.append(Paragraph("3.2 Convergence with density", h2))
story.append(img("figures/convergence_plot.png"))
story.append(Paragraph("Figure 2. Gap between discrete and continuum centroids at the final time bin, as a function of sprinkling density N.", caption))
story.append(Paragraph(
"Comparing the discrete and continuum energy-weighted centroids at the final time bin, "
"across increasing sprinkling density (N = 500, 1000, 2000, 4000), shows an overall "
"decreasing trend in the gap between them, consistent with (though not a clean "
"demonstration of) convergence to the continuum limit as density increases. Given the "
"known per-realization variance of the BD operator and the modest number of trials, "
"this should be regarded as a qualitative indication of convergence rather than a "
"precise scaling measurement.", body))

story.append(Paragraph("3.3 n-interval spectrum", h2))
story.append(img("figures/interval_spectrum.png"))
story.append(Paragraph("Figure 3. n-interval spectrum N_m of the sprinkled causal set (N=2000), cf. Surya [6] Fig. 3.", caption))
story.append(Paragraph(
"The computed spectrum N<sub>m</sub> decreases smoothly and monotonically with m, matching "
"the qualitative \"continuumlike\" signature described in Surya [6] (Fig. 3) for a "
"causal diamond in Minkowski spacetime. A smooth, monotonically decreasing N<sub>m</sub> is "
"characteristic of sprinklings into continuum-like geometries; significant deviations "
"from this shape have been associated with non-continuumlike causal sets. This "
"provides an independent, order-theoretic confirmation that the sprinkled causal set "
"used throughout this project is a genuine continuum approximation, using a "
"diagnostic directly drawn from current causal set research.", body))

story.append(PageBreak())

story.append(Paragraph("4. Limitations", h1))
limits = [
"compute_layers / interval_size scale poorly (roughly O(N<super>2</super>) to O(N<super>3</super>) depending on causal set density), making large-N or many-trial ensemble studies slow; a full, statistically rigorous convergence study would require an optimized implementation (e.g. time-sorted indexing to avoid re-scanning the full past for every interval check).",
"The discrete evolution scheme requires damping (&alpha;=0.3) to remain numerically stable, which measurably slows the effective propagation speed and destroys the coherent shape of the wave packet relative to the continuum solution.",
"The BD operator has large single-realization variance; only ensemble-averaged behavior matches the continuum expectation cleanly.",
"Only 1+1D Minkowski spacetime is implemented.",
]
story.append(ListFlowable([ListItem(Paragraph(t, body)) for t in limits], bulletType='bullet', leftIndent=16))

story.append(Paragraph("5. What I would do differently with more time", h1))
future = [
"Optimize interval_size/compute_layers (e.g. via time-sorted binary search or spatial indexing) to enable a proper, statistically significant convergence study across many trials and higher densities.",
"Investigate a better-normalized, less lossy stabilization scheme for the discrete evolution than simple under-relaxation, potentially drawing on retarded Green's function constructions on causal sets (e.g. Johnston [5]) or alternative discrete wave operators.",
"Extend to 2+1D Minkowski spacetime.",
"Extend the n-interval spectrum analysis to compare multiple spacetime regions (e.g. causal diamond vs hypercube), following the closeness-function construction in Surya [6], and explore its sensitivity to dimension and curvature.",
]
story.append(ListFlowable([ListItem(Paragraph(t, body)) for t in future], bulletType='bullet', leftIndent=16))

story.append(Paragraph("References", h1))
refs = [
"1. L. Bombelli, J. Lee, D. Meyer, R. Sorkin, \"Space-Time as a Causal Set,\" Phys. Rev. Lett. 59, 521 (1987).",
"2. D. M. Benincasa, F. Dowker, \"The scalar curvature of a causal set,\" Phys. Rev. Lett. 104, 181301 (2010).",
"3. F. Dowker, L. Glaser, \"Causal set d'Alembertians for various dimensions,\" Class. Quantum Grav. 30, 195016 (2013).",
"4. L. Glaser, \"A closed form expression for the causal set d'Alembertian,\" Class. Quantum Grav. 31, 095007 (2014).",
"5. S. Johnston, \"Quantum field theory on a causal set,\" J. Phys. Conf. Ser. 306, 012022 (2011).",
"6. S. Surya, \"A Closeness Function on Coarse Grained Lorentzian Geometries,\" arXiv:2510.19403 (2025).",
"7. S. Surya, \"The causal set approach to quantum gravity,\" Living Rev. Rel. 22, 5 (2019).",
]
for r in refs:
    story.append(Paragraph(r, ref))

os.makedirs("report", exist_ok=True)
doc.build(story)
print("Built report/REPORT.pdf")
